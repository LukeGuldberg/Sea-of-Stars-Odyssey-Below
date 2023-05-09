#pragma once

#include "vec.h"
// forward declaration
class Color;
class Graphics;
class Tilemap;
class Sprite;
class Object;
class Engine;
class AnimatedSprite;

class Camera
{
public:
    Camera(Graphics &graphics, int tilesize);

    void move_to(const Vec<double> &new_location);
    Vec<int> world_to_screen(const Vec<double> &world_position) const;

    void render(const Vec<double> &position, const Color &color,
                bool filled = true) const;
    void render(const Tilemap &tilemap, bool grid_on = false) const;
    void render(const Vec<double> &position, const Sprite &sprite) const;
    void render(const std::vector<std::pair<Sprite, int>> &backgrounds, const Engine &engine, Vec<double> position) const;
    void render(const Object &object) const;
    void render(Vec<double> position, AnimatedSprite &animatedsprite);
    void render(const AnimatedSprite &animatedsprite, const Engine &engine) const;
    void render(const std::vector<std::pair<Sprite, int>> &backgrounds) const;

    void render_life(int life, int max_life);
    void render_stars(int stars, int max_stars);

private:
    Graphics &graphics;
    int tilesize;
    Vec<double> location; // camera position in world coordinates

    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
};
