#pragma once

#include "object.h"
#include "enemytype.h"
#include "command.h"

class Engine;

class Enemy : public Object
{
public:
    Enemy(const Vec<double> &position, const Vec<int> &size, EnemyType &type);

    std::unique_ptr<Command> update(Engine &engine, double dt);
    std::unique_ptr<Command> next_action(Engine &engine);

    Vec<double> last_edge_position;
    Vec<int> size;
    EnemyType type;
};