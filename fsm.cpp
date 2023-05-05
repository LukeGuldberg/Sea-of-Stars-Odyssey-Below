#include "fsm.h"

#include "command.h"
#include "engine.h"
#include "player.h"

#include "world.h"
#include "randomness.h"
#include <iostream>
////////////////
// State
////////////////
std::unique_ptr<State> State::update(Player &player, Engine &engine,
                                     double dt)
{
    Physics old = player.physics;
    player.physics.update(dt);

    // attempt to move in x first
    Vec<double> future{player.physics.position.x, old.position.y};
    Vec<double> vx{player.physics.velocity.x, 0};
    engine.world->move_to(future, player.size, vx);

    // attempt to move y
    Vec<double> vy{0, player.physics.velocity.y};
    future.y = player.physics.position.y;
    engine.world->move_to(future, player.size, vy);

    // update position and velocity of player
    player.physics.position = future;
    player.physics.velocity = {vx.x, vy.y};

    // see if we touch a tile that has a command
    auto command = engine.world->touch_tiles(player);
    if (command)
    {
        command->execute(player, engine);
    }
    return nullptr;
}

////////////
// Standing
////////////
std::unique_ptr<State> Standing::handle_input(Player &player,
                                              const SDL_Event &event)
{

    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping>();
        }
        else if (key == SDLK_RIGHT)
        {
            return std::make_unique<Running>(player.walk_acceleration);
        }
        else if (key == SDLK_LEFT)
        {
            return std::make_unique<Running>(-player.walk_acceleration);
        }

        else if (key == SDLK_q)
        {
            return std::make_unique<AttackAll>();
        }
        else if (key == SDLK_f)
        {
            Vec<double> position{player.physics.position.x + player.size.x, player.physics.position.y + player.size.y}; // top corner of player
            Vec<double> velocity{5, 15};
            velocity.x += randint(-1, 1);
            velocity.y += randint(-1, 1);
            if (player.sprite.flip)
            {
                Vec<double> position{player.physics.position.x, player.physics.position.y + player.size.y}; // top corner of player
                velocity.x *= -1;
            }
            player.next_command = std::make_unique<FireProjectile>(player.arrow, position, velocity);
        }
    }
    return nullptr;
}

bool on_platform(const Player &player, const World &world)
{
    constexpr double epsilon = 1e-2;
    Vec<double> left_foot{player.physics.position.x,
                          player.physics.position.y - epsilon};
    Vec<double> right_foot{player.physics.position.x + player.size.x,
                           player.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

std::unique_ptr<State> Standing::update(Player &player, Engine &engine,
                                        double dt)
{
    State::update(player, engine, dt);
    player.physics.velocity.x *= damping; // physics::damp()
    player.standing.update(dt);
    player.sprite = player.standing.get_sprite();
    return nullptr;
}

void Standing::enter(Player &player)
{
    player.next_command = std::make_unique<Stop>();
    player.standing.reset();
    player.standing.flip(player.sprite.flip);
}

//////////////
// Jumping
//////////////
std::unique_ptr<State> Jumping::handle_input(Player &player,
                                             const SDL_Event &event)
{
    // reduced movement in left/right directions
    // space -> add a small vy
    // down -> drop faster
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping>();
        }
        else if (key == SDLK_RIGHT)
        {
            return std::make_unique<Running>(player.walk_acceleration);
        }
        else if (key == SDLK_LEFT)
        {
            return std::make_unique<Running>(-player.walk_acceleration);
        }
    }

    return nullptr;
}

std::unique_ptr<State> Jumping::update(Player &player, Engine &engine,
                                       double dt)
{
    State::update(player, engine, dt);
    player.jumping.update(dt);

    if (falling_frame_count < 60)
    {
        player.falling.update(dt);
    }
    // std::cout << jump_frame_count << '\n';
    if (player.physics.velocity.y > 0)
    {
        player.sprite = player.jumping.get_sprite();
    }
    else
    {
        falling_frame_count++;
        player.sprite = player.falling.get_sprite();
    }

    // apply friction
    if (on_platform(player, *engine.world))
    {
        return std::make_unique<Standing>();
    }
    return nullptr;
}

void Jumping::enter(Player &player)
{
    player.next_command = std::make_unique<Jump>(player.jump_velocity);
    player.jumping.flip(player.sprite.flip);
    int angle = 5;
    int max_angle = 60;
    player.falling.rotate(angle, max_angle);
}

void Jumping::exit(Player &player)
{
    falling_frame_count = 0;
    player.falling.reset();
}
///////////////
// Running
///////////////
Running::Running(double acceleration) : acceleration{acceleration} {}

std::unique_ptr<State> Running::handle_input(Player &,
                                             const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping>();
        }
    }
    if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT || key == SDLK_LEFT)
        {
            return std::make_unique<Standing>();
        }
    }

    return nullptr;
}

std::unique_ptr<State> Running::update(Player &player, Engine &engine,
                                       double dt)
{
    State::update(player, engine, dt);
    player.physics.apply_friction(damping); // friction
    player.running.update(dt);

    // if (!on_platform(player, *engine.world))
    if (player.physics.velocity.y > -2)
    {
        player.sprite = player.running.get_sprite();
    }
    else
    {
        falling_frame_count++;
        if (falling_frame_count < 45)
        {
            player.falling.update(dt);
        }
        player.sprite = player.falling.get_sprite();
    }
    return nullptr;
}

void Running::enter(Player &player)
{
    player.falling.reset();
    player.next_command = std::make_unique<Run>(acceleration);
    player.running.flip(acceleration < 0);
    player.falling.flip(acceleration < 0);
    falling_frame_count = 0;
    int angle = 5;
    int max_angle = 60;
    player.falling.rotate(angle, max_angle);
}

void Running::exit(Player &player)
{
    player.physics.acceleration.x = 0;
    falling_frame_count = 0;
    player.falling.reset();
}

//////////////////
/// AttackAll
//////////////////

std::unique_ptr<State> AttackAll::handle_input(Player &,
                                               const SDL_Event &event)
{
    if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_q)
        {
            return std::make_unique<Standing>();
        }
    }
    return nullptr;
}
std::unique_ptr<State> AttackAll::update(Player &player, Engine &engine,
                                         double)
{
    for (auto enemy : engine.world->enemies)
    {
        player.combat.attack(*enemy);
        if (enemy.get()->combat.health <= 0)
        {
            enemy.get()->type.animation = enemy.get()->type.death_sprite;
        }
        else
        { // make a state for taking damage
            enemy.get()->type.animation = enemy.get()->type.attacked_sprite;
        }
    }
    return std::make_unique<Standing>();
}

////////////
// Hurting
////////////
Hurting::Hurting(double elapsed_time) : elapsed_time{elapsed_time} {}
std::unique_ptr<State> Hurting::handle_input(Player &player, const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_LEFT)
        {

            return std::make_unique<Running_Hurt>(-player.walk_acceleration, elapsed_time);
        }
        else if (key == SDLK_RIGHT)
        {

            return std::make_unique<Running_Hurt>(player.walk_acceleration, elapsed_time);
        }
        else if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping_Hurt>(elapsed_time);
        }
    }
    if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_LEFT || SDLK_RIGHT)
        {
            return std::make_unique<Hurting>(elapsed_time);
        }
    }
    return nullptr;
}
std::unique_ptr<State> Hurting::update(Player &player, Engine &engine, double dt)
{
    State::update(player, engine, dt);
    player.physics.apply_friction(damping);
    player.standing_dmg.update(dt);
    elapsed_time += dt;
    if (elapsed_time >= cooldown)
    {
        return std::make_unique<Standing>();
    }
    player.sprite = player.standing_dmg.get_sprite();
    return nullptr;
}
void Hurting::enter(Player &player)
{
    player.next_command = std::make_unique<Stop>();
    player.combat.invincible = true;
    // player.standing_dmg.reset();
    // player.standing_dmg.flip(player.sprite.flip);
}
void Hurting::exit(Player &player)
{
    player.combat.invincible = false;
}

/////////////////
// Running_Hurt
/////////////////
Running_Hurt::Running_Hurt(double acceleration, double elapsed_time) : acceleration{acceleration}, elapsed_time{elapsed_time} {}

std::unique_ptr<State> Running_Hurt::handle_input(Player &, const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping_Hurt>(elapsed_time);
        }
    }
    if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT || key == SDLK_LEFT)
        {
            return std::make_unique<Hurting>(elapsed_time);
        }
    }

    return nullptr;
}

std::unique_ptr<State> Running_Hurt::update(Player &player, Engine &engine, double dt)
{
    State::update(player, engine, dt);
    player.physics.apply_friction(damping);
    player.running_dmg.update(dt);
    elapsed_time += dt;
    if (elapsed_time >= cooldown)
    {
        return std::make_unique<Standing>();
    }

    player.sprite = player.running_dmg.get_sprite();
    return nullptr;
}
void Running_Hurt::enter(Player &player)
{
    player.next_command = std::make_unique<Run>(acceleration);
    player.running_dmg.flip(acceleration < 0);
    player.combat.invincible = true;
}
void Running_Hurt::exit(Player &player)
{
    player.physics.acceleration.x = 0;
    player.combat.invincible = false;
}

////////////////
// Jumping_Hurt
////////////////
Jumping_Hurt::Jumping_Hurt(double elapsed_time) : elapsed_time{elapsed_time} {}

std::unique_ptr<State> Jumping_Hurt::handle_input(Player &player, const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP)
        {
            return std::make_unique<Jumping_Hurt>(elapsed_time);
        }
        else if (key == SDLK_RIGHT)
        {
            return std::make_unique<Running_Hurt>(player.walk_acceleration, elapsed_time);
        }
        else if (key == SDLK_LEFT)
        {
            return std::make_unique<Running_Hurt>(-player.walk_acceleration, elapsed_time);
        }
    }
    return nullptr;
}
std::unique_ptr<State> Jumping_Hurt::update(Player &player, Engine &engine, double dt)
{
    State::update(player, engine, dt);
    player.jumping_dmg.update(dt);

    if (on_platform(player, *engine.world))
    {
        return std::make_unique<Hurting>(elapsed_time);
    }
    if (elapsed_time >= cooldown)
    {
        return std::make_unique<Standing>();
    }
    player.sprite = player.jumping_dmg.get_sprite();
    return nullptr;
}
void Jumping_Hurt::enter(Player &player)
{
    player.next_command = std::make_unique<Jump>(player.jump_velocity);
    player.jumping_dmg.flip(player.sprite.flip);
    player.combat.invincible = true;
}
void Jumping_Hurt::exit(Player &player)
{
    player.combat.invincible = false;
}