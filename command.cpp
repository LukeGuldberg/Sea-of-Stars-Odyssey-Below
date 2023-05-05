#include "command.h"

#include "engine.h"
#include "object.h"
#include "projectile.h"
#include "world.h"
#include "vec.h"
////////////////////
// Stop
////////////////////
void Stop::execute(Object &object, Engine &engine)
{
    // object.color = {125, 0, 125, 255};
    // object.physics.velocity.y = 0;
    object.physics.acceleration.x = 0;
    engine.audio.play_sound("standing");
}

////////////////////
// Jump
////////////////////
Jump::Jump(double velocity) : velocity{velocity} {}

void Jump::execute(Object &object, Engine &engine)
{
    // object.color = {0, 0, 255, 255};
    object.physics.velocity.y = velocity;
    engine.audio.play_sound("jumping");
}

/////////////////
// Run
/////////////////

Run::Run(double acceleration) : acceleration{acceleration} {}
void Run::execute(Object &object, Engine &engine)
{
    // object.color = {255, 0, 0, 255};
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

LoadLevel::LoadLevel(const std::string &filename)
    : filename{filename} {}

void LoadLevel::execute(Object &, Engine &engine)
{
    engine.next_level = "assets\n" + filename;
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
    else if (command_name == "load_level")
    {
        if (arguments.size() != 1)
        {
            throw std::runtime_error("load_level must be followed by a level filename");
        }
        return std::make_shared<LoadLevel>(arguments.front());
    }

    throw std::runtime_error("Unknown command: " + command_name);
}