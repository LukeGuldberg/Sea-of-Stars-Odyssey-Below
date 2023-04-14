#include "graphics.h"
#include "randomness.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

Graphics::Graphics(const std::string &title, int window_width,
                   int window_height)
    : width{window_width}, height{window_height}
{
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result < 0)
    {
        std::cout << SDL_GetError() << "\n";
    }
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    if (!window)
    {
        std::cout << SDL_GetError() << "\n";
    }
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // initialize SDL, create a window and renderer
    // make sure to check all return values and throw exceptions when errors
    // occur

    if (!renderer)
    {
        throw std::runtime_error(SDL_GetError());
    }

    // SDL2 image
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags))
    {
        throw std::runtime_error(IMG_GetError());
    }
}

Graphics::~Graphics()
{
    // clean up: release SDL resources
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::load_spritesheet(const std::string &filename)
{
    std::ifstream input{filename};
    if (!input)
    {
        throw std::runtime_error("Cannot open filename: " + filename);
    }
    std::string image_filename;
    input >> image_filename;
    auto i = filename.find('/');
    std::string parent_path{filename.substr(0, i + 1)}; //  assets/
    image_filename = parent_path + image_filename;      //  assets/spritesheet.png

    int texture_id = get_texture_id(image_filename);

    // load sprites -> unordered_map <strings, sprites>
    std::string name;
    int x, y, width, height, scale;
    while (input >> name >> x >> y >> width >> height >> scale)
    {
        Vec shift{-width / 2, -height};
        Vec center{width / 2, height / 2};

        int number_of_frames;
        if (!(input >> number_of_frames))
        {
            number_of_frames = 1;
            input.clear(); // clear the error
        }
        for (int i = 0; i < number_of_frames; ++i)
        {
            Vec location{x + i * width, y};
            Vec size{width, height};
            Sprite sprite{texture_id, location, size, scale, shift, center};
            sprites[name].push_back(sprite);
        }
    }
}

Sprite Graphics::get_sprite(const std::string &name) const
{
    auto i = sprites.find(name);
    if (i == sprites.end())
    {
        throw std::runtime_error("Cannot find sprite: " + name);
    }
    return i->second.front();
}

AnimatedSprite Graphics::get_animated_sprite(const std::string &sprite_name, double dt_per_frame, bool random_start, bool shuffle_order)
{
    auto i = sprites.find(sprite_name);
    if (i == sprites.end())
    {
        throw std::runtime_error("Cannot find animated sprite " + sprite_name);
    }
    std::vector<Sprite> sprite = i->second;
    if (shuffle_order)
    {
        shuffle(std::begin(sprite), std::end(sprite));
    }

    if (sprites.size() > 1 && random_start)
    {
        // choose random start frame

        int starting_frame = randint(0, sprites.size() - 1);
        return AnimatedSprite{sprite, dt_per_frame, starting_frame};
    }
    else
    {
        return AnimatedSprite{sprite, dt_per_frame};
    }
}

Sprite Graphics::load_image(const std::string &filename)
{
    int id = get_texture_id(filename);
    SDL_Texture *texture = textures.at(id);
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    Sprite sprite{id, {0, 0}, {width, height}};
    return sprite;
}

void Graphics::clear()
{
    // clear the screen by painting it black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Graphics::draw(const SDL_Rect &rect, const Color &color, bool filled)
{
    // draw a filled rectangle
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue,
                           color.alpha);
    if (filled)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Graphics::update()
{
    // show the current canvas on the screen
    SDL_RenderPresent(renderer);
}

int Graphics::get_texture_id(const std::string &image_filename)
{
    auto search = texture_ids.find(image_filename);
    if (search != texture_ids.end())
    {
        int texture_id = search->second;
        return texture_id;
    }
    else
    {
        SDL_Texture *texture =
            IMG_LoadTexture(renderer, image_filename.c_str());
        if (!texture)
        {
            throw std::runtime_error(IMG_GetError());
        }

        // register the new texture
        int texture_id = textures.size();
        texture_ids[image_filename] = texture_id;

        // retain ownership of texture pointers
        textures.push_back(texture);
        return texture_id;
    }
}

void Graphics::draw_sprite(const Vec<int> &pixel, const Sprite &sprite)
{
    if (sprite.texture_id < 0)
    { // sprite with empty texture
        return;
    }
    // Calculate where sprite should appear on screen taking into account the
    // scale factor (image size -> screen size)
    int x = pixel.x + sprite.shift.x * sprite.scale;
    int y = pixel.y + sprite.shift.y * sprite.scale;
    int w = sprite.size.x * sprite.scale;
    int h = sprite.size.y * sprite.scale;
    SDL_Rect screen_pixels{x, y, w, h};

    // Calculate the center of the scaled up sprite
    SDL_Point center{sprite.center.x * sprite.scale,
                     sprite.center.y * sprite.scale};
    SDL_Rect image_pixels{sprite.location.x, sprite.location.y, sprite.size.x,
                          sprite.size.y};
    // Get the sprite's SDL texture
    SDL_Texture *texture = textures.at(sprite.texture_id);
    SDL_RendererFlip flip = sprite.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Draw the sprite on screen taking into account rotation (sprite.angle)
    // about its center, and whether to flip the sprite horizontally
    SDL_RenderCopyEx(renderer, texture, &image_pixels, &screen_pixels,
                     sprite.angle, &center, flip);
}