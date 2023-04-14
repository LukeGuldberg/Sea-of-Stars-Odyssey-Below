#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite(const std::vector<Sprite> &sprites, double dt_per_frame, int starting_frame)
    : sprites{sprites}, dt_per_frame{dt_per_frame}, time{0}, current_frame{starting_frame} {}

void AnimatedSprite::flip(bool flip)
{
    for (auto &sprite : sprites)
    {
        sprite.flip = flip;
    }
}

void AnimatedSprite::rotate(int angle, int max_angle)
{
    for (auto &sprite : sprites)
    {
        if (sprite.angle < max_angle)
        {
            sprite.angle += angle;
        }
    }
}

void AnimatedSprite::update(double dt)
{
    time += dt;
    if (time >= dt_per_frame)
    {
        time -= dt_per_frame;
        current_frame = (current_frame + 1) % sprites.size();
    }
}

void AnimatedSprite::reset()
{
    time = 0;
    current_frame = 0;
    for (auto &sprite : sprites)
    {
        sprite.angle = 0;
    }
}

Sprite AnimatedSprite::get_sprite() const
{
    return sprites.at(current_frame);
}

int AnimatedSprite::number_of_frames() const
{
    return sprites.size();
}

double AnimatedSprite::get_current_frame()
{
    return current_frame;
}