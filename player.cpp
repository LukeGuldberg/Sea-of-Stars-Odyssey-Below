#include "player.h"

#include <algorithm>

#include "world.h"
constexpr double walk_acceleration = 70;
constexpr double terminal_velocity = 300;
constexpr double jump_velocity = 100;
constexpr double gravity = 50;
constexpr double damping = 0.9;

Player::Player(const Vec<double>& position, const Vec<int>& size)
    : position{position}, size{size} {
    acceleration.y = gravity;
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
            velocity.y = -jump_velocity;
            acceleration.y = gravity;
        }
    }
    if (event.type == SDL_KEYUP) {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT) {
            acceleration.x = 0;
        } else if (key == SDLK_LEFT) {
            acceleration.x = 0;
        }
    }
}

void Player::update(World& world, double dt) {
    // make copy of physics components
    Vec<double> ftr_vel = velocity;
    Vec<double> ftr_acc = acceleration;
    Vec<double> ftr_pos = position;

    // update physics, semi-implicit euler
    ftr_vel += acceleration * dt;
    ftr_vel.x *= damping;  // "friction"
    // keep velocity under terminal
    ftr_vel.x = std::clamp(ftr_vel.x, -terminal_velocity, terminal_velocity);
    ftr_vel.y = std::clamp(ftr_vel.y, -terminal_velocity, terminal_velocity);
    ftr_pos += ftr_vel * dt;
    SDL_Rect future{static_cast<int>(ftr_pos.x), static_cast<int>(position.y),
                    size.x, size.y};

    // test x intersections first
    if (world.has_any_intersections(future)) {
        // collided
        acceleration.x = 0;
        velocity.x = 0;
    } else {
        acceleration.x = ftr_acc.x;
        velocity.x = ftr_vel.x;
        position.x = ftr_pos.x;
    }
    // test y intersections
    future.x = static_cast<int>(position.x);
    future.y = static_cast<int>(ftr_pos.y);
    if (world.has_any_intersections(future)) {
        // collided
        acceleration.y = ftr_acc.y;
        velocity.y = 0;
    } else {
        acceleration.y = ftr_acc.y;
        velocity.y = ftr_vel.y;
        position.y = ftr_pos.y;
    }
}

std::pair<SDL_Rect, Color> Player::get_sprite() const {
    int x = static_cast<int>(position.x);  // round down
    int y = static_cast<int>(position.y);
    SDL_Rect bounding_box{x, y, size.x, size.y};
    return {bounding_box, {255, 0, 255, 255}};
}
