#pragma once
#include <memory>
#include <vector>
#include <string>
#include "vec.h"
#include "projectile.h"
class Object;
class World;
class Engine;

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Object &object, Engine &engine) = 0;
};

class Stop : public Command
{
public:
    void execute(Object &object, Engine &engine) override;
};

class Jump : public Command
{
public:
    Jump(double velocity);
    void execute(Object &object, Engine &engine) override;

private:
    double velocity;
};

class Run : public Command
{
public:
    Run(double acceleration);
    void execute(Object &object, Engine &engine) override;

private:
    double acceleration;
};

class FireProjectile : public Command
{
public:
    FireProjectile(Projectile projectile, Vec<double> position, Vec<double> velocity);
    void execute(Object &object, Engine &engine) override;

private:
    Projectile projectile;
};

class EndGame : public Command
{
public:
    void execute(Object &object, Engine &engine) override;
};

class PlaySound : public Command
{
public:
    PlaySound(std::string sound_name, bool is_background);
    void execute(Object &object, Engine &engine) override;

private:
    std::string sound_name;
    bool is_background;
};

class LoadLevel : public Command
{
public:
    LoadLevel(const std::string &filename);
    void execute(Object &object, Engine &engine);

private:
    std::string filename;
};

std::shared_ptr<Command> create_command(std::string command_name, std::vector<std::string> arguments);