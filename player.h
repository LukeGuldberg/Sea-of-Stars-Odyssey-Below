#pragma once

#include "graphics.h"
#include <SDL2/SDL.h>

// forward declaration
class World;

class Player {
public:
    Player(int x, int y, int size);
    void handle_input(const SDL_Event& event);
    void update(World& world);
    std::pair<SDL_Rect, Color> get_sprite() const;
    
private:
    SDL_Rect bounding_box;
    int vx, vy;
};
