#include "tilemap.h"

#include <sstream>
#include <stdexcept>
Tilemap::Tilemap(int width, int height)
    : width{width}, height{height}, tiles{width * height} {
    if (width < 1) {
        throw std::runtime_error("width must be positive");
    }
    if (height < 1) {
        throw std::runtime_error("height must be positive");
    }
    std::fill(std::begin(tiles), std::end(tiles), Tile::Open);
}

const Tile& Tilemap::operator()(int x, int y) const {
    check_bounds(x, y);
    return tiles.at(x + y * width);
}

Tile& Tilemap::operator()(int x, int y) {
    check_bounds(x, y);
    return tiles.at(x + y * width);
}

void Tilemap::check_bounds(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ") is not within bounds (";
        ss << width << ", " << height << ")";
        throw std::runtime_error(ss.str());
    }
}
