#include "world.h"

#include <array>
#include <cmath>

World::World(int width, int height) : tilemap(width, height) {}

void World::add_platform(int x, int y, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x + j, y + i) = Tile::Platform;
        }
    }
}

void World::move_to(Vec<double>& position, const Vec<int>& size,
                    Vec<double>& velocity) {
    // test sides first, if both collide then move backwards

    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({position.x, position.y + size.y}) &&
             collides({position.x + size.x, position.y + size.y})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    else if (collides(position) &&
             collides({position.x, position.y + size.y})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({position.x + size.x, position.y}) &&
             collides({position.x + size.x, position.y + size.y})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }

    // test corners next, move back in smaller axis
    else if (collides(position)) {
        double dx = std::ceil(position.x) - position.x;
        double dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x, position.y + size.y})) {
        double dx = std::ceil(position.x) - position.x;
        double dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        } else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x + size.x, position.y})) {
        double dx = position.x - std::floor(position.x);
        double dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x + size.x, position.y + size.y})) {
        double dx = position.x - std::floor(position.x);
        double dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        } else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}

bool World::collides(const Vec<double>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y) == Tile::Platform;
}
