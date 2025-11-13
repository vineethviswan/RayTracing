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
    // returns a random double value [0,1)
    /*std::uniform_real_distribution<double> distr(0.0, 1.0);
    std::mt19937 generator;
    return distr(generator);*/

    return rand () / (RAND_MAX + 1.0);
}

inline double RandomDouble (double min, double max)
{
    // returns a random double value [0,1)
    /*std::uniform_real_distribution<double> distr(min, max);
    std::mt19937 generator;
    return distr(generator);*/
    return min + (max - min) * RandomDouble ();
}

#endif
