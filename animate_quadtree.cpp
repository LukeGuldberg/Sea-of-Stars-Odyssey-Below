#include <SDL_events.h>
#include "quadtree.h"
#include "graphics.h"
#include "randomness.h"
#include "vec.h"
#include <algorithm>
#include <chrono>

void draw_AABB(Graphics &graphics, AABB boundary, const Color &color = {255, 255, 255, 255});
void draw_point(Graphics &graphics, Vec<double> point, const Color &color = {255, 0, 0, 255});
void draw_quadtree(Graphics &graphics, const QuadTree &quadtree);

int main()
{
    Graphics graphics{"QuadTree demo", 1280, 720};
    std::vector<Vec<double>> points;
    ;
    auto generate_point = []()
    {
        double x = randint(100, 1100);
        double y = randint(100, 600);
        return Vec<double>{x, y};
    };

    AABB boundary{{1280.0 / 2, 720.0 / 2}, {580, 300}};
    QuadTree quadtree{boundary};

    double dt{0.2};
    double elapsed{0};
    auto previous = std::chrono::high_resolution_clock::now();
    bool running{true};
    while (running)
    {
        auto current = std::chrono::high_resolution_clock::now();
        elapsed += std::chrono::duration<double>(current - previous).count();
        previous = current;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        if (elapsed > dt)
        {
            elapsed -= dt;

            // add point
            auto point = generate_point();
            points.push_back(point);
            quadtree.insert(point);
        }

        graphics.clear();
        draw_quadtree(graphics, quadtree);
        for (auto point : points)
        {
            draw_point(graphics, point);
        }
        graphics.update();
    }
}

void draw_AABB(Graphics &graphics, AABB boundary, const Color &color)
{
    Vec<double> xy = boundary.center - boundary.half_dimension;
    Vec<double> size = boundary.half_dimension * 2.0;
    SDL_Rect rect{static_cast<int>(xy.x), static_cast<int>(xy.y),
                  static_cast<int>(size.x), static_cast<int>(size.y)};
    graphics.draw(rect, color, false);
}

void draw_point(Graphics &graphics, Vec<double> point, const Color &color)
{
    SDL_Rect rect{static_cast<int>(point.x), static_cast<int>(point.y), 5, 5};
    graphics.draw(rect, color, true);
}

void draw_quadtree(Graphics &graphics, const QuadTree &quadtree)
{
    draw_AABB(graphics, quadtree.boundary);
    if (quadtree.north_west)
    {
        draw_quadtree(graphics, *quadtree.north_west);
        draw_quadtree(graphics, *quadtree.north_east);
        draw_quadtree(graphics, *quadtree.south_west);
        draw_quadtree(graphics, *quadtree.south_east);
    }
}
