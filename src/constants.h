#pragma once
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double deg_to_rad(double deg) { return deg * pi / 180.0; }

inline double rnd()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float rnd(double _min, double _max)
{
    static std::uniform_real_distribution<double> distribution(_min, _max);
    static std::mt19937 generator;
    return distribution(generator);
}

#include "util/rgb.h"
#include "util/interval.h"
#include "util/ray.h"
#include "util/vec3.h"
