#pragma once

#include <SDL2/SDL.h>
#include <sprite.h>

#include <string>
#include <unordered_map>
#include <vector>
#include "animatedsprite.h"
#include "vec.h"

class Color
{
public:
    int red{255}, green{0}, blue{255}, alpha{255};
};

class Graphics
{
public:
    Graphics(const std::string &title, int window_width, int window_height);
    ~Graphics();

    void load_spritesheet(const std::string &filename);
    Sprite get_sprite(const std::string &name) const;
    AnimatedSprite get_animated_sprite(const std::string &name, double dt_per_frame, bool random_start = false, bool shuffle_order = false) const;
    void clear();
    void draw(const SDL_Rect &rect, const Color &color, bool filled = true);
    Sprite load_image(const std::string &filename);
    void draw_sprite(const Vec<int> &pixel, const Sprite &sprite);
    void update();
    const int width, height;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::vector<SDL_Texture *> textures;
    std::unordered_map<std::string, int> texture_ids;

    std::unordered_map<std::string, std::vector<Sprite>> sprites;

    int get_texture_id(const std::string &image_filename);
};
