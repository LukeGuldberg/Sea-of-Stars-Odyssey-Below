#include "quadtree.h"
#include <iostream>

int main()
{
    AABB box{{0.5, 0.5}, {0.5, 0.5}};

    double x, y;
    std::cout << "Enter a point: ";
    while (std::cin >> x >> y)
    {
        Vec<double> point{x, y};
        std::cout << std::boolalpha << box.contains(point) << '\n';
    }
}