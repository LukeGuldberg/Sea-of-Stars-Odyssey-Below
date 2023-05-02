#include "world.h"
#include "level.h"
#include "player.h"
#include <array>
#include <cmath>
World::World(const Level &level)
    : tilemap{level.width, level.height}, backgrounds{level.backgrounds}, animated_background{level.animated_background}, quadtree{AABB{{level.width / 2.0, level.height / 2.0}, {level.width / 2.0, level.height / 2.0}}}
{
    for (auto [position, tile] : level.tiles)
    {
        tilemap(position.x, position.y) = tile;
    }

    for (auto [position, type] : level.enemies)
    {
        enemies.push_back(std::make_shared<Enemy>(position, Vec<int>{1, 1}, type));
    }
}

std::shared_ptr<Command> World::touch_tiles(const Player &player)
{
    int x = std::floor(player.physics.position.x);
    int y = std::floor(player.physics.position.y);
    const Vec<int> &size = player.size;
    const std::vector<Vec<int>> displacemnts{{0, 0}, {size.x, 0}, {0, size.y}, {size.x, size.y}};
    for (const Vec<int> &displacemnt : displacemnts)
    {
        Tile &tile = tilemap(x + displacemnt.x, y + displacemnt.y);
        if (tile.command)
        {
            auto command = tile.command;
            tile.command = nullptr;
            return command;
        }
    }
    return nullptr;
}

void World::move_to(Vec<double> &position, const Vec<int> &size,
                    Vec<double> &velocity)
{
    // test sides first, if both collide then move backwards

    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y}))
    {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({position.x, position.y + size.y}) &&
             collides({position.x + size.x, position.y + size.y}))
    {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    else if (collides(position) &&
             collides({position.x, position.y + size.y}))
    {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({position.x + size.x, position.y}) &&
             collides({position.x + size.x, position.y + size.y}))
    {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }

    // test corners next, move back in smaller axis
    else if (collides(position))
    {
        double dx = std::ceil(position.x) - position.x;
        double dy = std::ceil(position.y) - position.y;
        if (dx > dy)
        {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else
        {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x, position.y + size.y}))
    {
        double dx = std::ceil(position.x) - position.x;
        double dy = position.y - std::floor(position.y);
        if (dx > dy)
        {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else
        {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y}))
    {
        double dx = position.x - std::floor(position.x);
        double dy = std::ceil(position.y) - position.y;
        if (dx > dy)
        {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else
        {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y + size.y}))
    {
        double dx = position.x - std::floor(position.x);
        double dy = position.y - std::floor(position.y);
        if (dx > dy)
        {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else
        {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}

bool World::collides(const Vec<double> &position) const
{
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y).blocking;
}

void World::build_quadtree()
{
    quadtree.clear();

    for (std::shared_ptr<Enemy> enemy : enemies)
    {
        quadtree.insert(enemy.get());
    }
}
