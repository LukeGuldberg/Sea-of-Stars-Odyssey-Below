#pragma once

#include "vec.h"

// represents a 2D image in the game
class Sprite
{
public:
    int texture_id{-1};      // assigned automatically when requested from graphics class
    Vec<int> location{0, 0}; // upper left corner of sprite in image file
    Vec<int> size{0, 0};     // size (width, height) of sprite in image file
    int scale{1};            // scale up original sprite size to display size
    Vec<int> shift{0, 0};    // pixels to shift by when displaying
    Vec<int> center{0, 0};   // position to rotate about
    double angle{0.0};       // angle of rotation
    bool flip{false};        // whether to flip sprite horizontally
};
