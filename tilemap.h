#pragma once
#include "sprite.h"
#include "command.h"
#include <vector>
#include <memory>

// enum class Tile { Open, Platform };

class Tile
{
public:
    Sprite sprite;
    bool blocking{false};
    std::shared_ptr<Command> command{nullptr};
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