#include "enemy.h"
#include "enemytype.h"
Enemy::Enemy(const Vec<double> &position, const Vec<int> &size, EnemyType &type)
    : last_edge_position{position}, size{size}, type{type}
{
    physics.position = position;
    physics.acceleration.x = type.walk_acceleration;
    sprite = type.standing.get_sprite();
}