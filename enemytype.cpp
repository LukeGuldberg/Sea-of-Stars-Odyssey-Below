#include "enemytype.h"
#include "enemy.h"

EnemyType create_enemy_type(Graphics &graphics, std::string type_name, double walk_acceleration)
{
    EnemyType type;
    type.walk_acceleration = walk_acceleration;
    type.standing = graphics.get_animated_sprite(type_name + "_standing", 0.5, true, false);
    type.running = graphics.get_animated_sprite(type_name + "_running", 0.5, true, false);

    return type;
}

std::unique_ptr<Command> default_behavior(Engine &, Enemy &enemy)
{
    if (abs(enemy.last_edge_position.x - enemy.physics.position.x) > 20)
    {
        enemy.last_edge_position.x = enemy.physics.position.x;
        enemy.physics.acceleration.x = -enemy.physics.acceleration.x;
    }
    return std::make_unique<Run>(enemy.physics.acceleration.x);
}