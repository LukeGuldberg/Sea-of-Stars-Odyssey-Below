#include "enemy.h"
#include "enemytype.h"
Enemy::Enemy(const Vec<double> &position, const Vec<int> &size, EnemyType &type)
    : last_edge_position{position}, size{size}, type{type}
{
    physics.position = position;
    physics.acceleration.x = type.walk_acceleration;
    sprite = type.standing.get_sprite();
}

std::unique_ptr<Command> Enemy::update(Engine &engine, double dt)
{
    physics.update(dt);
    physics.apply_friction(physics.damping);

    type.standing.update(dt);
    sprite = type.standing.get_sprite();
    return nullptr;
}

std::unique_ptr<Command> Enemy::next_action(Engine &engine)
{
    return type.behavior(engine, *this);
}
