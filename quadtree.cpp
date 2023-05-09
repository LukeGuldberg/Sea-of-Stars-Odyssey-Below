#include "quadtree.h"
#include "object.h"
#include <algorithm>

bool AABB::contains(const Vec<double> &point) const
{
    Vec<double> displacement = point - center;
    return std::abs(displacement.x) < half_dimension.x && std::abs(displacement.y) < half_dimension.y;
}

bool AABB::intersects(const AABB &other) const
{
    Vec<double> displacement = other.center - center;
    return std::abs(displacement.x) < (half_dimension.x + other.half_dimension.x) && std::abs(displacement.y) < (half_dimension.y + other.half_dimension.y);
}

QuadTree::QuadTree(AABB boundary)
    : boundary{boundary} {}

void QuadTree::clear()
{
    objects.clear();
    if (north_west)
    {
        north_west = nullptr;
    }
    if (north_east)
    {
        north_east = nullptr;
    }
    if (south_west)
    {
        south_west = nullptr;
    }
    if (south_east)
    {
        south_east = nullptr;
    }
}

bool QuadTree::insert(Object *object)
{
    // ignore objects that don't belong in here
    if (!boundary.contains(object->physics.position))
    {
        return false;
    }

    // if there is space and no children, object is stored here
    if (objects.size() < NODE_CAPACITY && north_west == nullptr)
    {
        objects.push_back(object);
        return true;
    }

    // otherwise subdivide and insert into one of the children
    if (north_west == nullptr)
    {
        subdivide();
    }

    return north_west->insert(object) || north_east->insert(object) || south_west->insert(object) || south_east->insert(object);
}

void QuadTree::subdivide()
{
    Vec<double> half = boundary.half_dimension / 2.0;

    north_west = std::make_shared<QuadTree>(AABB{{boundary.center.x - half.x, boundary.center.y + half.y}, half});
    north_east = std::make_shared<QuadTree>(AABB{{boundary.center.x + half.x, boundary.center.y + half.y}, half});
    south_west = std::make_shared<QuadTree>(AABB{{boundary.center.x - half.x, boundary.center.y - half.y}, half});
    south_east = std::make_shared<QuadTree>(AABB{{boundary.center.x + half.x, boundary.center.y - half.y}, half});

    for (Object *object : objects)
    {
        insert(object);
    }
    objects.clear();
}

std::vector<Object *> QuadTree::query_range(AABB range) const
{
    if (!boundary.intersects(range))
    {
        return {};
    }

    // handle leaf node
    std::vector<Object *> results;
    if (north_west == nullptr)
    {
        std::copy_if(std::begin(objects), std::end(objects), std::back_inserter(results),
                     [&](Object *object)
                     {
                         return range.contains(object->physics.position);
                     });
        return results;
    }

    // add objects from children
    auto pts = north_west->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = north_east->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = south_west->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = south_east->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));

    return results;
}
