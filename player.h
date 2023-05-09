#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "command.h"
#include "fsm.h"
#include "graphics.h"
#include "physics.h"
#include "vec.h"
#include "object.h"
#include "projectile.h"

// forward declaration
class World;
class Engine;

class Player : public Object
{
public:
    Player(Engine &engine, const Vec<double> &position, const Vec<int> &size);
    std::unique_ptr<Command> handle_input(const SDL_Event &event);
    void update(Engine &engine, double dt);
    std::pair<Vec<double>, Color> get_sprite() const;

    const double walk_acceleration = 18;
    const double jump_velocity = 5;
    const double crouch_acceleration = 10;
    const double crouch_jump_acceleration = 7;
    Vec<int> size;
    Color color{125, 0, 125, 255};
    AnimatedSprite standing, running, falling, jumping, standing_dmg, jumping_dmg, running_dmg;
    std::unique_ptr<State> state;
    std::unique_ptr<Command> next_command;

    Projectile arrow;

    int stars_found{0};
    int max_stars{3};
};
