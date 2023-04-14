#pragma once

#include "sprite.h"

#include <vector>

class AnimatedSprite
{
public:
    AnimatedSprite() {}
    AnimatedSprite(const std::vector<Sprite> &sprites, double dt_per_frame, int starting_frame = 0);
    void flip(bool flip);                  // flip sprite horizontally
    void rotate(int angle, int max_angle); // rotate each sprite in an animation
    void update(double dt);                // move to next frame of animation
    void reset();                          // set current_frame and time to zero
    Sprite get_sprite() const;
    int number_of_frames() const;
    double get_current_frame();

private:
    std::vector<Sprite> sprites;
    double dt_per_frame{0}, time{0};
    int current_frame{0};
};
