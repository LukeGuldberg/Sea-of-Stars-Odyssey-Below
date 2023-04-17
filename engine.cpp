#include "engine.h"

#include <chrono>

#include "player.h"
#include "settings.h"
#include "level.h"
Engine::Engine(const Settings &settings)
    : graphics{settings.title, settings.screen_width, settings.screen_height},
      camera{graphics, settings.tile_size}
{
    load_level(settings.starting_level);
}

void Engine::load_level(const std::string &level_filename)
{
    Level level{level_filename, graphics, audio};
    audio.play_sound("background", true);
    world = std::make_shared<World>(level);

    // load player
    player = std::make_shared<Player>(*this, level.player_start_location, Vec<int>{1, 1});

    // move camera to start position
    camera.move_to(player->physics.position);
}
//     // boundary walls
//     world.add_platform(0, 0, 30, 1);
//     world.add_platform(0, 0, 1, 16);
//     world.add_platform(30, 0, 1, 16);
//     world.add_platform(0, 15, 30, 1);

//     // platforms
//     world.add_platform(3, 7, 4, 1);
//     world.add_platform(13, 4, 6, 1);

//     // add player
//     player =
//         std::make_shared<Player>(*this, Vec<double>{10, 4}, Vec<int>{1, 1});

//     // move camera
//     camera.move_to(player->physics.position);
//     // std::queue<std::pair<double, std::unique_ptr<Command>>> script;
//     // script.push({3, std::make_unique<Accelerate>(40)});
//     // script.push({5, std::make_unique<Accelerate>(40)});
//     // script.push({7, std::make_unique<Accelerate>(40)});
// }
void Engine::run()
{
    running = true;
    constexpr double dt = 1.0 / 60.0;

    auto previous = std::chrono::high_resolution_clock::now();
    double lag{0.0};
    while (running)
    {
        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current - previous;
        previous = current;
        lag += elapsed.count();

        // move the player in the world
        input();
        while (lag >= dt)
        {
            update(dt);
            lag -= dt;
        }
        render();
    }
}

void Engine::stop()
{
    running = false;
}

void Engine::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // handle windows and systems events first
        if (event.type == SDL_QUIT)
        {
            running = false;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_g)
        {
            grid_on = !grid_on;
        }
        auto command = player->handle_input(event);
        if (command)
        {
            command->execute(*player, *this);
        }
        // pass the rest of the events to the player who will
        // react to keypresses by moving
        player->handle_input(event);
    }
    for (auto enemy : world->enemies)
    {
        auto command = enemy->next_action(*this);
        if (command)
        {
            command->execute(*enemy, *this);
        }
    }
}
void Engine::update(double dt)
{
    player->update(*this, dt);
    for (auto enemy : world->enemies)
    {
        auto command = enemy->update(*this, dt);
        if (command)
        {
            command->execute(*enemy, *this);
        }
    }
    camera.move_to(player->physics.position);

    // camera.update(dt);
}
void Engine::render()
{
    // draw the player and platforms
    graphics.clear();

    camera.render(world->backgrounds);
    auto [position, color] = player->get_sprite();
    camera.render(world->tilemap, grid_on);
    // camera.move_to(position);
    camera.render(position, color);
    camera.render(*player);
    for (auto enemy : world->enemies)
    {
        camera.render(*enemy);
    }
    // camera.render(position, player->sprite);
    graphics.update();
}