#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "command.h"
#include "tilemap.h"
#include "vec.h"
#include "enemy.h"
#include "quadtree.h"
#include "animatedobject.h"

class Level;
class Player;

class World
{
public:
    World(const Level &level);

    // void add_platform(int x, int y, int width, int height);
    void move_to(Vec<double> &position, const Vec<int> &size,
                 Vec<double> &velocity);
    bool collides(const Vec<double> &position) const;

    std::shared_ptr<Command> touch_tiles(const Player &player);

    Tilemap tilemap;
    std::vector<std::pair<Sprite, int>> backgrounds;
    AnimatedSprite animated_background;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<Projectile> projectiles;
    std::vector<AnimatedObject> animated_objects;
    Vec<double> load_level_position;
    std::shared_ptr<Tile> load_level_tile;
    int stars_found{0};
    int max_stars{3};

    void build_quadtree();
    void remove_inactive();

    QuadTree quadtree;
};
