#include "graphics.h"
#include "player.h"
#include "world.h"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    Graphics graphics{"game", 1280, 720};
    
    // objects
    Player player{600, 300, 64};
    
    World world;
    world.add_platform(320, 500, 512, 64);
    
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
        player.update(world);

        // draw the player and platforms
        graphics.clear();
        auto [box, color] = player.get_sprite();
        graphics.draw(box, color);
        for (const auto& platform : world.get_platforms()) {
            graphics.draw(platform, {0, 255, 0, 255});
        }
        graphics.update();
    }
}

