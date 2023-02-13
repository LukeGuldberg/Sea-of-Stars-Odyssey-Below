#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <iostream>

#include "graphics.h"
#include "player.h"
#include "vec.h"
#include "world.h"

int main() {
    Graphics graphics{"game", 1280, 720};

    // objects
    Player player{{600, 300}, {64, 64}};

    World world;
    world.add_platform(0, 656, 1280, 64);
    world.add_platform(200, 450, 400, 64);
    world.add_platform(600, 200, 250, 64);
    world.add_platform(0, 0, 64, 720);
    world.add_platform(1216, 0, 64, 720);

    bool running{true};
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // handle windows and systems events first
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            // pass the rest of the events to the player who will
            // react to keypresses by moving
            player.handle_input(event);
        }

        // move the player in the world
        constexpr double dt = 1.0 / 10.0;
        player.update(world, dt);

        // draw the player and platforms
        graphics.clear();
        auto [box, color] = player.get_sprite();
        graphics.draw(box, color);
        for (const auto& platform : world.get_platforms()) {
            graphics.draw(platform, {255, 255, 0, 255});
        }
        graphics.update();
    }
}
