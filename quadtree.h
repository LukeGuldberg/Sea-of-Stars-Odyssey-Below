#pragma once

#include <memory>
#include <vector>

#include "vec.h"

class Object;
// rectangular axis aligned bounding box
struct AABB
{
    Vec<double> center, half_dimension;

    bool contains(const Vec<double> &point) const;
    bool intersects(const AABB &other) const;
};

class QuadTree
{
public:
    QuadTree(AABB boundary);

    void clear();

    bool insert(Object *object);
    void subdivide();

    std::vector<Object *> query_range(AABB range) const;

    static constexpr std::size_t NODE_CAPACITY = 4;

    AABB boundary;
    std::vector<Object *> objects;
    std::shared_ptr<QuadTree> north_west, north_east, south_west, south_east;
};
