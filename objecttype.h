#pragma once
#include "animatedsprite.h"
#include "graphics.h"

class ObjectType
{
public:
    AnimatedSprite animation;
};

ObjectType create_object_type(Graphics &graphics, std::string type_name);
ObjectType create_star(Graphics &graphics);
ObjectType create_coin(Graphics &graphics);