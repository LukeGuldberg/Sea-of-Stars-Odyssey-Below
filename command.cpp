#include "command.h"

#include "engine.h"
#include "object.h"
#include "projectile.h"
#include "world.h"
#include "vec.h"
#include "player.h"
#include <iostream>
////////////////////
// Stop
////////////////////
void Stop::execute(Object &object, Engine &engine)
{
    object.physics.acceleration.x = 0;
    engine.audio.play_sound("standing");
}

////////////////////
// Jump
////////////////////
Jump::Jump(double velocity) : velocity{velocity} {}

void Jump::execute(Object &object, Engine &engine)
{
    object.physics.velocity.y = velocity;
    engine.audio.play_sound("jumping");
}

/////////////////
// Run
/////////////////

Run::Run(double acceleration) : acceleration{acceleration} {}
void Run::execute(Object &object, Engine &engine)
{
    object.physics.acceleration.x = acceleration;
    engine.audio.play_sound("swimming");
}
/////////////
// Projectile
////////////
FireProjectile::FireProjectile(Projectile projectile, Vec<double> position, Vec<double> velocity)
    : projectile{projectile}
{
    this->projectile.physics.position = position;
    this->projectile.physics.velocity = velocity;
    this->projectile.physics.acceleration.y = gravity;
}
void FireProjectile::execute(Object &, Engine &engine)
{
    engine.world->projectiles.push_back(projectile);
}

/////////////////
// Game Changes
/////////////////
void EndGame::execute(Object &, Engine &engine)
{
    engine.stop();
}

PlaySound::PlaySound(std::string sound_name, bool is_background)
    : sound_name{sound_name}, is_background{is_background} {}
void PlaySound::execute(Object &, Engine &engine)
{
    engine.audio.play_sound(sound_name, is_background);
}

LoadLevel::LoadLevel(int level_num) : level_num{level_num} {}

void LoadLevel::execute(Object &, Engine &engine)
{
    engine.world->level_number++;
    std::string filename = "assets/level-0";
    filename += std::to_string(engine.world->level_number);
    filename += ".txt";
    engine.next_level = filename;
}

StarFound::StarFound(const Vec<double> pos) : pos{pos} {}

void StarFound::execute(Object &, Engine &engine)
{
    if (engine.world->stars_found < engine.world->max_stars)
    {
        engine.world->stars_found++;
        engine.world->animated_objects.erase(std::remove_if(engine.world->animated_objects.begin(), engine.world->animated_objects.end(), [&](AnimatedObject animated_object) -> bool
                                                            { return animated_object.position == pos; }),
                                             engine.world->animated_objects.end());
    }
    if (engine.world->stars_found == engine.world->max_stars && (engine.world->level_number == 1 || engine.world->level_number == 2))
    {
        engine.world->load_level_tile.sprite = engine.graphics.get_sprite("open_treasure_chest");
        engine.world->load_level_tile.command = std::make_shared<LoadLevel>(level_num);
    }
    else if (engine.world->stars_found == engine.world->max_stars && engine.world->level_number == 3)
    {
        engine.world->load_level_tile.sprite = engine.graphics.get_sprite("open_treasure_chest");
        engine.world->load_level_tile.command = std::make_shared<EndGame>();
    }
}

void Spikes::execute(Object &, Engine &engine)
{
    engine.player->combat.take_damage(2);
    engine.player->state->exit(*engine.player);
    engine.player->state = std::make_unique<Hurting>(0.0);
    engine.player->state->enter(*engine.player);
}

std::shared_ptr<Command> create_command(std::string command_name, std::vector<std::string> arguments)
{
    if (command_name == "end_game")
    {
        return std::make_shared<EndGame>();
    }
    else if (command_name == "play_sound")
    {
        bool is_background = arguments.at(1) == "true" ? true : false;
        std::string sound_name = arguments.at(0);
        return std::make_shared<PlaySound>(sound_name, is_background);
    }
    else if (command_name == "spikes")
    {
        return std::make_shared<Spikes>();
    }
    throw std::runtime_error("Unknown command: " + command_name);
}