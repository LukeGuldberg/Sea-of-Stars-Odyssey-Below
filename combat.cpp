#include "combat.h"
#include "object.h"

void Combat::attack(Object &object)
{
    object.combat.take_damage(attack_damage);
}
void Combat::take_damage(int attack_damage)
{
    health -= attack_damage;
    if (health <= 0)
    {
        is_alive = false;
    }
}
