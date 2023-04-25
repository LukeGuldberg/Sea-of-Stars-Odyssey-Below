#pragma once
class Object;

class Combat
{
public:
    int health, max_health, attack_damage;
    bool is_alive = true;

    void attack(Object &object);
    void take_damage(int attack_damage);
};