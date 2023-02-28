#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "tilemap.h"
#include "vec.h"

class World {
public:
    World(int width, int height);

    void add_platform(int x, int y, int width, int height);
    void move_to(Vec<double>& position, const Vec<int>& size,
                 Vec<double>& velocity);
    bool collides(const Vec<double>& position) const;

    Tilemap tilemap;
};
