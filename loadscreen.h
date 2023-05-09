#pragma once
#include <vector>

#include "sprite.h"

class Graphics;
class Audio;

class Loadscreen {
public:
    Loadscreen(const std::string& filename, Graphics& graphics, Audio& audio);

    std::string filename;

    std::vector<std::pair<Sprite, int>> backgrounds;

private:
    void load(Graphics& graphics, Audio& audio);
};