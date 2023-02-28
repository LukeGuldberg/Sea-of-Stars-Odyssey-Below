#include "player.h"

#include <algorithm>

#include "world.h"
constexpr double walk_acceleration = 18;
constexpr double terminal_velocity = 70;
constexpr double jump_velocity = 4;
constexpr double gravity = -6;
constexpr double damping = 0.95;

Player::Player(const Vec<double>& position, const Vec<int>& size)
    : position{position}, size{size} {
    acceleration = {0, gravity};
    velocity = {0, 0};
}

// vel += accel * dt
// pos += vel * dt

void Player::handle_input(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT) {
            acceleration.x = walk_acceleration;
        } else if (key == SDLK_LEFT) {
            acceleration.x = -walk_acceleration;
        } else if (key == SDLK_SPACE) {
            velocity.y = jump_velocity;
            acceleration.y = gravity;
        }
    }
    if (event.type == SDL_KEYUP) {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT) {
            // velocity.x = 0;
            acceleration.x = 0;
        } else if (key == SDLK_LEFT) {
            // velocity.x = 0;
            acceleration.x = 0;
        }
    }
}

void Player::update(World& world, double dt) {
    // update physics, semi-implicit euler
    velocity += acceleration * dt;
    velocity.x *= damping;
    // keep velocity under terminal
    velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
    velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);

    Vec<double> displacement = velocity * dt;

    // attempt to move in x first
    Vec<double> future{position.x + displacement.x, position.y};
    Vec<double> vx{velocity.x, 0};
    world.move_to(future, size, vx);

    // attempt to move y
    Vec<double> vy{0, velocity.y};
    future.y += displacement.y;
    world.move_to(future, size, vy);

    // update position and velocity of player
    position = future;
    velocity.x = vx.x;
    velocity.y = vy.y;
}

std::pair<Vec<double>, Color> Player::get_sprite() const {
    return {position, {255, 0, 255, 255}};
}
