#pragma once

#include <SDL2/SDL.h>
#include <vector>

class World {
public:
    void add_platform(int x, int y, int width, int height);
    const std::vector<SDL_Rect>& get_platforms() const;
    bool has_any_intersections(const SDL_Rect& bounding_box) const;

private:
    std::vector<SDL_Rect> platforms;
};
