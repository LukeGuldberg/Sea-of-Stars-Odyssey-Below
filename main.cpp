#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine.h"
#include "settings.h"

int main() {
    Settings settings{
        "C:/Users/lukeg/code/GameDev/SDL-SDL2/Super-Awesome-"
        "Platformer/settings.txt"};
    Engine engine{settings};
    engine.run();
}
