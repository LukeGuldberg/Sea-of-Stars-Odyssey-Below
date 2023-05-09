#include "animatedobject.h"

AnimatedObject::AnimatedObject(const Vec<double> &position, const Vec<int> &size, ObjectType &type) : position{position}, size{size}, type{type} {}
void AnimatedObject::update(Engine &, double dt)
{
    type.animation.update(dt);
}
