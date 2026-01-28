#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>
#include "Constants.h"

class Interval
{
public:
    double Min, Max;

    Interval () : Min (+INFNTY), Max (-INFNTY) { } // Default interval is empty

    Interval (double min, double max) : Min (min), Max (max) { }

    double Size () const { return Max - Min; }

    bool Contains (double x) const { return Min <= x && x <= Max; }

    bool Surrounds (double x) const { return Min < x && x < Max; }

    //static const Interval EMPTY, UNIVERSE;
};

//const Interval Interval::EMPTY = Interval (+INFNTY, -INFNTY);
//const Interval Interval::UNIVERSE = Interval (-INFNTY, +INFNTY);

#endif
