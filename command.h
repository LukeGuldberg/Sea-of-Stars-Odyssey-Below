#pragma once

class Player;
class World;
class Engine;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Player& player, Engine& engine) = 0;
};

class Stop : public Command {
public:
    void execute(Player& player, Engine& engine) override;
};

class Jump : public Command {
public:
    Jump(double velocity);
    void execute(Player& player, Engine& engine) override;

private:
    double velocity;
};

class Run : public Command {
public:
    Run(double acceleration);
    void execute(Player& player, Engine& engine) override;

private:
    double acceleration;
};
