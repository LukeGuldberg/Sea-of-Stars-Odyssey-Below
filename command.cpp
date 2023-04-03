#include "command.h"

#include "engine.h"
#include "player.h"
#include "world.h"
////////////////////
// Stop
////////////////////
void Stop::execute(Player &player, Engine &engine)
{
    player.color = {125, 0, 125, 255};
    // player.physics.velocity.y = 0;
    player.physics.acceleration.x = 0;
}

////////////////////
// Jump
////////////////////
Jump::Jump(double velocity) : velocity{velocity} {}

void Jump::execute(Player &player, Engine &engine)
{
    player.color = {0, 0, 255, 255};
    player.physics.velocity.y = velocity;
    // engine.audio.play_sound("jumping");
}

/////////////////
// Run
/////////////////

Run::Run(double acceleration) : acceleration{acceleration} {}
void Run::execute(Player &player, Engine &engine)
{
    player.color = {255, 0, 0, 255};
    player.physics.acceleration.x = acceleration;
}