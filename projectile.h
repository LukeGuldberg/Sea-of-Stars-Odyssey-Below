#pragma once

#include "object.h"

class Engine;

class Projectile : public Object
{
public:
    void update(Engine &engine, double dt);
    Vec<int> size{1, 1};
    double lifetime{1}, elapse{0};
};