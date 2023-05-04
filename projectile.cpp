#include "projectile.h"
#include <cmath>

#include "engine.h"
void Projectile::update(Engine &engine, double dt)
{
    Physics old = physics;
    physics.update(dt);

    // Check for collisions
    Vec<double> future{physics.position.x, old.position.y};
    Vec<double> vx{physics.velocity.x, 0};
    engine.world->move_to(future, size, vx);

    // attempt to move y
    Vec<double> vy{0, physics.velocity.y};
    future.y = physics.position.y;
    engine.world->move_to(future, size, vy);

    // update position & velocity
    physics.position = future;
    physics.velocity = {vx.x, vy.y};

    // rotate arrow if it is still moving
    if (physics.velocity.x != 0 && physics.velocity.y != 0)
    {
        sprite.angle = 90 - std::atan2(physics.velocity.y, physics.velocity.x) * 180 / 3.14159;
    }

    // collided with something so stop moving
    if (physics.velocity.x == 0 || physics.velocity.y == 0)
    {
        physics.velocity.x = physics.velocity.y = 0;
        physics.acceleration.y = 0;
    }
    if (physics.velocity.x == 0 && physics.velocity.y == 0)
    {
        combat.attack_damage = 0;
        elapse += dt;
        if (elapse >= lifetime)
        {
            combat.is_alive = false;
        }
    }
}