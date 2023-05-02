#include "quadtree.h"
#include "randomness.h"
#include <algorithm>
#include <chrono>
#include <iostream>
class Timer {
public:
    Timer() {
        start();
    }
    void start() {
        t0 = std::chrono::high_resolution_clock::now();
    }
    double stop() {
        t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = t1 - t0;
        return elapsed.count();
    }
    
    std::chrono::time_point<std::chrono::high_resolution_clock> t0, t1;
};

double random_double(double min, double max) {
    std::uniform_real_distribution<double> dist{min, max};
    return dist(random_engine);
}

int main() {
    int N = 10000;
    double xmax = 10;
    double ymax = 10;
    std::vector<Vec<double>> points(N);
    std::generate(std::begin(points), std::end(points),
                  [=]() {
                      double x = random_double(0, xmax);
                      double y = random_double(0, ymax);
                      return Vec<double>{x, y};
                  });

    
    AABB boundary{{xmax/2, ymax/2}, {xmax/2, ymax/2}};
    QuadTree quadtree{boundary};
    Timer timer;
    timer.start();

    for (int i = 0; i < 100; ++i) {
        quadtree.clear();
        for (const auto& point : points) {
            quadtree.insert(point);
        }
    }
    double elapsed = timer.stop() / 100;
    std::cout << "Build time: " << elapsed << "\n";

    timer.start();
    for (int i = 0; i < 100; ++i) {
        AABB search_range{{200, 300}, {100, 100}};
        auto found = quadtree.query_range(search_range);
    }
    elapsed = timer.stop() / 100;
    std::cout << "Search time: " << elapsed << "\n";
}

