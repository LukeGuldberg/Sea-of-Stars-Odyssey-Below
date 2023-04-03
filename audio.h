#pragma once
#include <SDL2/SDL_mixer.h>

#include <string>
#include <unordered_map>

class Audio
{
public:
    Audio();
    ~Audio();

    void load_sounds(const std::string &filename);
    void play_sound(const std::string &sound_name, bool is_background = false);

private:
    std::unordered_map<std::string, Mix_Chunk *> sounds;
};