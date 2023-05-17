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

class Standing_Hurt : public State
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

class Hurting : public State
{
public:
    Hurting(double elapsed_time);
    std::unique_ptr<State> handle_input(Player &player, const SDL_Event &event) override;
    std::unique_ptr<State> update(Player &player, Engine &engine, double dt) override;
    void enter(Player &player) override;
    void exit(Player &player) override;

    double elapsed_time;
    const double cooldown{2};
};

class Running_Hurt : public State
{
public:
    Running_Hurt(double acceleration, double elapsed_time);
    std::unique_ptr<State> handle_input(Player &player, const SDL_Event &event) override;
    std::unique_ptr<State> update(Player &player, Engine &engine, double dt) override;
    void enter(Player &player) override;
    void exit(Player &player) override;

    double acceleration;
    double elapsed_time;
    const double cooldown{2};
};
class Jumping_Hurt : public State
{
public:
    Jumping_Hurt(double elapsed_time);
    std::unique_ptr<State> handle_input(Player &player, const SDL_Event &event) override;
    std::unique_ptr<State> update(Player &player, Engine &engine, double dt) override;
    void enter(Player &player) override;
    void exit(Player &player) override;

    double elapsed_time;
    const double cooldown{2};
};