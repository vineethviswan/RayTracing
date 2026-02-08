#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <limits>
#include <memory>
#include <random>

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double PI = 3.1415926535897932385;

inline double DegreesToRadian (double degrees) { return degrees * PI / 180.0; }

inline double RandomDouble ()
{    
    static thread_local std::mt19937 generator (std::random_device {}());
    static std::uniform_real_distribution<double> distr (0.0, 1.0);
    return distr (generator);
}

inline double RandomDouble (double min, double max)
{
    // returns a random double value [0,1)
    return min + (max - min) * RandomDouble ();
}

inline double LinearToGamma (double linear_component)
{
    if (linear_component > 0)
        return std::sqrt (linear_component);

    return 0;
}

#endif
