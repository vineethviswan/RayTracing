#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vector3.h"

class sphere : public Hittable
{
public:
    sphere (const Point3 &center, double radius) : center (center), radius (std::fmax (0, radius)) { }

    bool Hit (const Ray &r, Interval ray_t, HitRecord &rec) const override
    {
        Vector3 oc = center - r.origin ();
        auto a = r.direction ().LengthSquared ();
        auto h = Dot (r.direction (), oc);
        auto c = oc.LengthSquared () - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt (discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.Surrounds (root))
        {
            root = (h + sqrtd) / a;
            if (!ray_t.Surrounds (root))
                return false;
        }

        rec.t = root;
        rec.p = r.at (rec.t);
        Vector3 outward_normal = (rec.p - center) / radius;
        rec.SetFaceNormal (r, outward_normal);

        return true;
    }

private:
    Point3 center;
    double radius;
};

#endif
