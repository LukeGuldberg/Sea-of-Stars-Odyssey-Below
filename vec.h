#pragma once
#include <functional>
#include <sstream>
#include <stdexcept>
template <typename T>
class Vec
{
public:
    Vec(T x = T{}, T y = T{}) : x{x}, y{y} {}
    T x, y;
};

template <typename T>
Vec<T> &operator+=(Vec<T> &left, const Vec<T> &right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

template <typename T>
Vec<T> operator+(Vec<T> left, const Vec<T> &right)
{
    return left += right;
    return left;
}

template <typename T>
Vec<T> &operator-=(Vec<T> &left, const Vec<T> &right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

template <typename T>
Vec<T> operator-(Vec<T> left, const Vec<T> &right)
{
    return left -= right;
}

template <typename T>
Vec<T> &operator*=(Vec<T> &left, const Vec<T> &right)
{
    left.x *= right.x;
    left.y *= right.y;
    return left;
}

template <typename T>
Vec<T> operator*(Vec<T> left, const Vec<T> &right)
{
    return left *= right;
}

template <typename T>
Vec<T> operator*(Vec<T> left, T scalar)
{
    return left *= Vec{scalar, scalar};
}

template <typename T>
Vec<T> operator*(T scalar, const Vec<T> *right)
{
    return right * scalar;
}

template <typename T>
Vec<T> &operator/=(Vec<T> &left, T scalar)
{
    if (scalar == 0)
    {
        std::stringstream msg{"Cannot divide "};
        msg << left << " by " << scalar;
        throw std::runtime_error(msg.str());
    }
    left.x /= scalar;
    left.y /= scalar;
    return left;
}

template <typename T>
Vec<T> operator/(Vec<T> left, T scalar)
{
    return left /= scalar;
}

template <typename T>
bool operator==(const Vec<T> &left, const Vec<T> &right)
{
    return left.x == right.x && left.y == right.y;
}

template <typename T>
bool operator!=(const Vec<T> &left, const Vec<T> &right)
{
    return !(left == right);
}

// TODO add distance between vectors
template <typename T>
std::ostream &operator<<(std::ostream &os, const Vec<T> vec)
{
    return os << "(" << vec.x << ", " << vec.y << ")";
}

namespace std
{
    template <>
    struct hash<Vec<int>>
    {
        void hash_combine(std::size_t &seed, int value) const
        {
            seed ^= hash<int>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        std::size_t operator()(const Vec<int> &v) const
        {
            std::size_t seed = 0;
            hash_combine(seed, v.x);
            hash_combine(seed, v.y);
            return seed;
        }
    };
} // namespace std