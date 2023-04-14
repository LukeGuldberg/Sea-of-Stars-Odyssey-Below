#pragma once

#include "object.h"
#include "enemytype.h"
class Engine;
class EnemyType;

class Enemy : public Object
{
public:
    Enemy(const Vec<double> &position, const Vec<int> &size, EnemyType &type);

    Vec<double> last_edge_position;
    Vec<int> size;
    EnemyType type;
};