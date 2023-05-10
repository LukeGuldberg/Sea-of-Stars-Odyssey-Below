#pragma once

#include <memory>
#include <audio.h>
#include <optional>
#include "camera.h"
#include "graphics.h"
#include "world.h"
#include "level.h"

// forward declare
class Player;
class Settings;
class Level;

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
    // Level level;

    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;
    std::optional<std::string> next_level;

private:
    bool running{true};
    bool grid_on{false};

    void input();
    void update(double dt);
    void render();
    void setup_end_screen();
};