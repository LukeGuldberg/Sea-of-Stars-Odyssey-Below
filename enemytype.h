#pragma once
#include <functional>
#include "animatedsprite.h"
#include "command.h"
#include "graphics.h"

class Enemy;
class Engine;

class EnemyType
{
public:
    AnimatedSprite standing, running;
    double walk_acceleration;
    int health, damage;
    std::function<std::unique_ptr<Command>(Engine &engine, Enemy &enemy)> behavior;
};

EnemyType create_enemy_type(Graphics &graphics, std::string type_name, double walk_acceleration);
std::unique_ptr<Command> default_behavior(Engine &, Enemy &enemy);