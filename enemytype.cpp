#include "enemytype.h"

EnemyType create_enemy_type(Graphics &graphics, std::string type_name, double walk_acceleration)
{
    EnemyType type;
    type.walk_acceleration = walk_acceleration;
    type.standing = graphics.get_animated_sprite(type_name + "_standing", 0.5, true, false);
    type.running = graphics.get_animated_sprite(type_name + "_running", 0.5, true, false);

    return type;
}