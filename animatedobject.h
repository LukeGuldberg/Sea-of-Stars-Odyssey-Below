#pragma once
#include "objecttype.h"
#include "vec.h"
#include "object.h"

class Engine;

class AnimatedObject
{
public:
    AnimatedObject(const Vec<double> &position, const Vec<int> &size, ObjectType &type);

    Vec<double> position;
    Vec<int> size;
    ObjectType type;

    void update(Engine &engine, double dt);
};