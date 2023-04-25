#pragma once

#include "physics.h"
#include "sprite.h"
#include "combat.h"

class Object
{
public:
    Physics physics;
    Sprite sprite;
    Combat combat;
};