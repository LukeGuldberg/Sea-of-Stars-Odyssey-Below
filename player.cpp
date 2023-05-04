#include "player.h"

#include <algorithm>
#include <memory>

#include "engine.h"
#include "world.h"

Player::Player(Engine &engine, const Vec<double> &position,
               const Vec<int> &size)
    : size{size}
{
    physics.position = position;
    combat.health = 100;
    combat.max_health = 100;
    combat.attack_damage = 3;
    state = std::make_unique<Standing>();
    state->enter(*this);
    standing = engine.graphics.get_animated_sprite("woodcutter_standing", .35, false, false);

    jumping = engine.graphics.get_animated_sprite("woodcutter_jumping", .35, false, false);

    running = engine.graphics.get_animated_sprite("woodcutter_swimming", .25, false, false);

    falling = engine.graphics.get_animated_sprite("woodcutter_falling", .25, false, false);

    standing_dmg = engine.graphics.get_animated_sprite("woodcutter_standing_dmg", .25, false, false);

    running_dmg = engine.graphics.get_animated_sprite("woodcutter_swimming_dmg", .25, false, false);

    jumping_dmg = engine.graphics.get_animated_sprite("woodcutter_jumping_dmg", .25, false, false);

    sprite = standing.get_sprite();

    arrow.sprite = engine.graphics.get_sprite("arrow");
    arrow.combat.invincible = true;
    arrow.combat.attack_damage = 1;
}

// vel += accel * dt
// pos += vel * dt

std::unique_ptr<Command> Player::handle_input(const SDL_Event &event)
{
    // state -> handle_input(*this, event);

    auto new_state = state->handle_input(*this, event);
    if (new_state)
    {
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }

    auto next = std::move(next_command);
    next_command = nullptr;
    return next;
}

void Player::update(Engine &engine, double dt)
{
    auto new_state = state->update(*this, engine, dt);
    if (new_state)
    {
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }

    if (next_command)
    {
        next_command->execute(*this, engine);
        next_command = nullptr;
    }
}

std::pair<Vec<double>, Color> Player::get_sprite() const
{
    return {physics.position, color};
}
