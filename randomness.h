#pragma once

#include <algorithm>
#include <random>

static std::mt19937 random_engine{std::random_device{}()};

int randint(int min, int max); // inclusive

template <typename Iterator>
void shuffle(Iterator begin, Iterator end)
{
    std::shuffle(begin, end, random_engine);
}
