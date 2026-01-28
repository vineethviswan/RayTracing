#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"

class HitRecord
{
public:
    Point3 p;
    Vector3 normal;
    double t;
    bool front_face;

    void SetFaceNormal (const Ray &r, const Vector3 &outward_normal)
    {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = Dot (r.direction (), outward_normal) < 0;
        normal = front_face ? outward_normal : -1 * outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable () = default;

    virtual bool Hit (const Ray &r, Interval ray_t, HitRecord &rec) const = 0;
};

#endif
