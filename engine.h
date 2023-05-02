#pragma once

#include <memory>
#include <audio.h>
#include "camera.h"
#include "graphics.h"
#include "world.h"

// forward declare
class Player;
class Settings;

class Engine
{
public:
    Engine(const Settings &settings);

    void load_level(const std::string &level_filename);
    void run();
    void stop();
    Audio audio;
    Graphics graphics;
    Camera camera;

    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;

private:
    bool running{true};
    bool grid_on{false};

    void input();
    void update(double dt);
    void render();
};