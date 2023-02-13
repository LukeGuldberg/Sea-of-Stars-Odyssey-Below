#pragma once

#include <SDL2/SDL.h>

#include "graphics.h"
#include "vec.h"

// forward declaration
class World;

class Player {
public:
    Player(const Vec<double>& position, const Vec<int>& size);
    void handle_input(const SDL_Event& event);
    void update(World& world, double dt);
    std::pair<SDL_Rect, Color> get_sprite() const;

private:
    Vec<double> position;
    Vec<int> size;
    Vec<double> velocity, acceleration;
};
