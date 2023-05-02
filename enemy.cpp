#include "enemy.h"
#include "enemytype.h"
#include "engine.h"

Enemy::Enemy(const Vec<double> &position, const Vec<int> &size, EnemyType &type)
    : last_edge_position{position}, size{size}, type{type}
{
    physics.position = position;
    physics.acceleration = type.acceleration;
    combat.health = type.health;
    combat.max_health = type.health;
    combat.attack_damage = type.damage;
    sprite = type.animation.get_sprite();
}

std::unique_ptr<Command> Enemy::update(Engine &engine, double dt)
{
    Physics old = physics;
    physics.update(dt);
    // physics.apply_friction(damping);
    // Check for collisions
    // attempt to move in x
    Vec<double> future{physics.position.x, old.position.y};
    Vec<double> vx{physics.velocity.x, 0};
    engine.world->move_to(future, size, vx);
    // attempt to move in y
    Vec<double> vy{0, physics.velocity.y};
    future.y = physics.position.y;
    engine.world->move_to(future, size, vy);
    // update position and velocity
    physics.position = future;
    physics.velocity = {vx.x, vy.y};
    // check for collision with wall
    if (vx.x == 0 && physics.acceleration.x != 0)
    {
        type.animation.flip(-physics.acceleration.x < 0);
        last_edge_position = physics.position;
        return std::make_unique<Run>(-physics.acceleration.x);
    }
    type.animation.flip(physics.acceleration.x < 0);
    type.animation.update(dt);
    sprite = type.animation.get_sprite();
    return nullptr;
}

std::unique_ptr<Command> Enemy::next_action(Engine &engine)
{
    return type.behavior(engine, *this);
}
