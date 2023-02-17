#pragma once

#include <SDL2/SDL.h>
#include <string>

class Color {
public:
    int red{255}, green{0}, blue{255}, alpha{255};
};

class Graphics {
public:
    Graphics(const std::string& title, int window_width, int window_height);
    ~Graphics();

    void clear();
    void draw(const SDL_Rect& rect, const Color& color);
    void update();
    const int width, height;
private:
    
    SDL_Window* window;
    SDL_Renderer* renderer;
};
