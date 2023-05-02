#include <SDL_events.h>
#include "quadtree.h"
#include "graphics.h"
#include "randomness.h"
#include <algorithm>

void draw_AABB(Graphics& graphics, AABB boundary, const Color& color={255, 255, 255, 255});
void draw_point(Graphics& graphics, Vec<double> point, const Color& color={255, 0, 0, 255});
void draw_quadtree(Graphics& graphics, const QuadTree& quadtree);


int main() {
    Graphics graphics{"QuadTree demo", 1280, 720};
    int N = 100;
    
    std::vector<Vec<double>> points(N);
    std::generate(std::begin(points), std::end(points),
                  []() {
                      double x = randint(100, 1100);
                      double y = randint(100, 600);
                      return Vec<double>{x, y};
                  });

    AABB boundary{{1280.0/2, 720.0/2}, {580, 300}};
    QuadTree quadtree{boundary};
    for (const auto& point : points) {
        quadtree.insert(point);
    }

    AABB search_range{{200, 300}, {100, 100}};
    auto found = quadtree.query_range(search_range);
    
    bool running{true};
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        graphics.clear();
        draw_quadtree(graphics, quadtree);
        for (const Vec<double>& point : points) {
            draw_point(graphics, point);
        }
        draw_AABB(graphics, search_range, {0, 255, 0, 255});
        for (auto point : found) {
            draw_point(graphics, point, {0, 255, 255, 255});
        }
        graphics.update();
    }
}

void draw_AABB(Graphics& graphics, AABB boundary, const Color& color) {
    Vec<double> xy = boundary.center - boundary.half_dimension;
    Vec<double> size = 2.0 * boundary.half_dimension;
    SDL_Rect rect{static_cast<int>(xy.x), static_cast<int>(xy.y),
                  static_cast<int>(size.x), static_cast<int>(size.y)};
    graphics.draw(rect, color, false);
}

void draw_point(Graphics& graphics, Vec<double> point, const Color& color) {
    SDL_Rect rect{static_cast<int>(point.x), static_cast<int>(point.y), 5, 5};
    graphics.draw(rect, color, true);
}

void draw_quadtree(Graphics& graphics, const QuadTree& quadtree) {
    draw_AABB(graphics, quadtree.boundary);
    if (quadtree.north_west) {
        draw_quadtree(graphics, *quadtree.north_west);
        draw_quadtree(graphics, *quadtree.north_east);
        draw_quadtree(graphics, *quadtree.south_west);
        draw_quadtree(graphics, *quadtree.south_east);
    }
}
