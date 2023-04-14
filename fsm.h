#pragma once
#include <SDL2/SDL.h>

#include <memory>

#include "vec.h"
class Player;
class World;
class Engine;

class State
{
public:
    virtual ~State() {}
    virtual std::unique_ptr<State> handle_input(Player &player,
                                                const SDL_Event &event) = 0;
    virtual std::unique_ptr<State> update(Player &player, Engine &engine,
                                          double dt);
    virtual void enter(Player &) {} // add engine to every enter state so that you can play a sound
    virtual void exit(Player &) {}
};

class Standing : public State
{
public:
    std::unique_ptr<State> handle_input(Player &player,
                                        const SDL_Event &event) override;
    bool on_platform(const Player &player, const World &world);
    std::unique_ptr<State> update(Player &player, Engine &engine,
                                  double dt) override;
    void enter(Player &player) override;
};

class Jumping : public State
{
public:
    std::unique_ptr<State> handle_input(Player &player,
                                        const SDL_Event &event) override;
    std::unique_ptr<State> update(Player &player, Engine &engine,
                                  double dt) override;
    void enter(Player &player) override;
    void exit(Player &player) override;

    int falling_frame_count = 0;
};

class Running : public State
{
public:
    Running(double acceleration);
    std::unique_ptr<State> handle_input(Player &player,
                                        const SDL_Event &event) override;
    std::unique_ptr<State> update(Player &player, Engine &engine,
                                  double dt) override;
    void enter(Player &player) override;
    void exit(Player &player) override;

private:
    double acceleration;
    int falling_frame_count;
};

// class Crouching : public State {
// public:
//     std::unique_ptr<State> handle_input(Player& player,
//                                         const SDL_Event& event) override;
//     std::unique_ptr<State> update(Player& player, World& world,
//                                   double dt) override;
//     void enter(Player& player) override;
// };

// class Sinking : public State {
// public:
//     std::unique_ptr<State> handle_input(Player& player,
//                                         const SDL_Event& event) override;
//     std::unique_ptr<State> update(Player& player, World& world,
//                                   double dt) override;
//     void enter(Player& player) override;
// };