#include "player.h"

#include "world.h"

Player::Player(int x, int y, int size)
    : bounding_box{x, y, size, size}, vx{0}, vy{0} {}

void Player::handle_input(const SDL_Event& event) {
    // change the player's velocity based on the keypress
    // make sure to set it to zero if no buttons are pressed
    SDL_Keycode key = event.key.keysym.sym;

    if (key == SDLK_DOWN) {
        vx = 0;
        vy = 20;
    } else if (key == SDLK_UP) {
        vx = 0;
        vy = -20;
    } else if (key == SDLK_LEFT) {
        vx = -20;
        vy = 0;
    } else if (key == SDLK_RIGHT) {
        vx = 20;
        vy = 0;
    } else {
        vx = 0;
        vy = 0;
    }
}

void Player::update(World& world) {
    // predict the player's future position: make copy of bounding box, move it
    SDL_Rect future = bounding_box;

    // test for a collision and conditionally move the player's position
    // you can optionally set the velocity back to zero
    future.x += vx;
    future.y += vy;
    if (!world.has_any_intersections(future)) {
        bounding_box = future;
    } else {
        vx = 0;
        vy = 0;
    }
}

std::pair<SDL_Rect, Color> Player::get_sprite() const {
    return {bounding_box, {255, 0, 0, 255}};
}
