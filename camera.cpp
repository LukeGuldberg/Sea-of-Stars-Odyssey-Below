#include "camera.h"

#include <algorithm>

#include "graphics.h"
#include "object.h"
#include "tilemap.h"
#include "engine.h"
#include "animatedsprite.h"
Camera::Camera(Graphics &graphics, int tilesize)
    : graphics{graphics}, tilesize{tilesize}
{
    calculate_visible_tiles();
}
void Camera::move_to(const Vec<double> &new_location)
{
    location.x = new_location.x;
    location.y = new_location.y;
    // location.y = std::clamp(location.y, 5.0, static_cast<double>(visible_max.y));
    // location.y = std::clamp(location.y, 5.0, 12.0);
    location.y = std::clamp(location.y, 5.0, static_cast<double>(graphics.height));

    location.x = std::clamp(location.x, 14.0, static_cast<double>(visible_max.x));
    location.x = std::clamp(location.x, -14.0, static_cast<double>(visible_max.x));

    calculate_visible_tiles();
}

Vec<int> Camera::world_to_screen(const Vec<double> &world_position) const
{
    Vec<double> scaled = (world_position - location) * static_cast<double>(tilesize); // mapping my location to pixel location
    Vec<int> pixel{static_cast<int>(scaled.x), static_cast<int>(scaled.y)};

    // shift to the center of the screen
    pixel += Vec<int>{graphics.width / 2, graphics.height / 2};

    // flip the y axis
    pixel.y = graphics.height - pixel.y;
    return pixel;
}

void Camera::render(const Vec<double> &position, const Color &color,
                    bool filled) const
{
    Vec<int> pixel = world_to_screen(position);
    pixel -= Vec{tilesize / 2, tilesize / 2}; // centered on tile
    SDL_Rect rect{pixel.x, pixel.y, tilesize, tilesize};
    graphics.draw(rect, color, filled);
}

void Camera::render(const Tilemap &tilemap, bool grid_on) const
{
    // screen to world conversion
    // calculate min and max world coordinates and only draw those
    int xmin = std::max(0, visible_min.x);
    int ymin = std::max(0, visible_min.y);
    int xmax = std::min(visible_max.x, tilemap.width - 1);
    int ymax = std::min(visible_max.y, tilemap.height - 1);

    // draw tiles
    for (int y = ymin; y <= ymax; ++y)
    {
        for (int x = xmin; x <= xmax; ++x)
        {
            const Tile &tile = tilemap(x, y);
            Vec<double> position{static_cast<double>(x), static_cast<double>(y)};
            render(position, tile.sprite);
            if (grid_on)
            {
                render(position, Color{0, 0, 0, 255}, false);
            }
        }
    }
}

void Camera::render(const std::vector<std::pair<Sprite, int>> &backgrounds, const Engine &engine, Vec<double> position) const
{
    auto pos = engine.camera.world_to_screen({position.x - 12, 22});
    for (auto [sprite, distance] : backgrounds)
    {
        graphics.draw_sprite({pos.x, pos.y}, sprite);
    }
}

void Camera::render(const Vec<double> &position, const Sprite &sprite) const
{
    Vec<int> pixel = world_to_screen(position);
    pixel.y += tilesize / 2; // shift sprite's bottom center down to bottom of tile
    graphics.draw_sprite(pixel, sprite);
}

void Camera::render(const Object &object) const
{
    render(object.physics.position, object.sprite);
}

void Camera::render(const AnimatedSprite &animatedsprite, const Engine &engine) const
{
    auto pos = engine.camera.world_to_screen({0, 18});
    graphics.draw_sprite({pos.x, pos.y}, animatedsprite.get_sprite());
}

void Camera::render_life(int life, int max_life)
{
    Sprite heart = graphics.get_sprite("heart");
    Sprite empty_heart = graphics.get_sprite("empty_heart");

    for (int i = 0; i < life; ++i)
    {
        Vec<int> position{35, 65};
        position.x += i * 64 + 10;
        graphics.draw_sprite(position, heart);
    }

    for (int i = life; i < max_life; ++i)
    {
        Vec<int> position{35, 65};
        position.x += i * 64 + 10;
        graphics.draw_sprite(position, empty_heart);
    }
}

void Camera::calculate_visible_tiles()
{
    // number of tiles visible(plus one for the edges)
    Vec<int> num_tiles =
        Vec{graphics.width, graphics.height} / (tilesize * 2) + Vec{7, 7};
    Vec<int> center{static_cast<int>(location.x), static_cast<int>(location.y)};
    visible_max = center + num_tiles;
    visible_min = center - num_tiles;
}
