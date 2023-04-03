#pragma once
#include "sprite.h"
#include <vector>

// enum class Tile { Open, Platform };

class Tile
{
public:
    Sprite sprite;
    bool blocking{false};
};

class Tilemap
{
public:
    Tilemap(int width, int height);

    const Tile &operator()(int x, int y) const; // getter (RHS)
    Tile &operator()(int x, int y);             // setter (LHS)

    const int width;
    const int height;

private:
    std::vector<Tile> tiles;

    void check_bounds(int x, int y) const;
};