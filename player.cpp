#include "player.h"

#include "world.h"

Player::Player(int x, int y, int size)
    : bounding_box{x, y, size, size}, vx{0}, vy{0} {}

void Player::handle_input(const SDL_Event& event) {
    // change the player's velocity based on the keypress
    // make sure to set it to zero if no buttons are pressed
}

void Player::update(World& world) {
    // predict the player's future position: make copy of bounding box, move it

    // test for a collision and conditionally move the player's position
    // you can optionally set the velocity back to zero
}

std::pair<SDL_Rect, Color> Player::get_sprite() const {
    return {bounding_box, {255, 0, 0, 255}};
}
