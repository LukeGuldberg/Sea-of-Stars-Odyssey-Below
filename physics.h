#pragma once

#include "vec.h"
constexpr double terminal_velocity = 70;
constexpr double gravity = -4;

class Physics {
public:
    void update(double dt);
    void apply_friction(double friction);
    Vec<double> position, velocity, acceleration{0, gravity};

    const double damping = 0.95;
};