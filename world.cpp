#include "world.h"

#include <SDL2/SDL_rect.h>

#include <algorithm>

void World::add_platform(int x, int y, int width, int height) {
    SDL_Rect platform{x, y, width, height};
    platforms.push_back(platform);
}

const std::vector<SDL_Rect>& World::get_platforms() const {
    return platforms;
}

bool World::has_any_intersections(const SDL_Rect& bounding_box) const {
    // test if the given bounding_box intersects any of the platforms
    for (SDL_Rect platform : platforms) {
        if (SDL_HasIntersection(&platform, &bounding_box)) {
            return true;
        }
    }
    return false;
}
