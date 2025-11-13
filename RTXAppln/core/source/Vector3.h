#pragma once

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cmath>
#include <iostream>
#include "Utility.h"

class Vector3
{
public:
    Vector3 () : m_x (0.0), m_y (0.0), m_z (0.0) { }
    Vector3 (double x, double y, double z) : m_x (x), m_y (y), m_z (z) { }

    double GetX () const { return m_x; }
    double GetY () const { return m_y; }
    double GetZ () const { return m_z; }

    double Length () const;
    double LengthSquared () const;
    bool NearZero () const;

    Vector3 &operator*= (const double t);
    Vector3 &operator+= (const Vector3 &vec);
    Vector3 &operator/= (double t);

    static Vector3 RandomVector ();
    static Vector3 RandomVector (double min, double max);

    double m_x;
    double m_y;
    double m_z;
};

// Vector Utility Functions
inline std::ostream &operator<< (std::ostream &out, const Vector3 &v)
{
    return out << v.m_x << ' ' << v.m_y << ' ' << v.m_z;
}

inline Vector3 operator+ (const Vector3 &u, const Vector3 &v)
{
    return Vector3 (u.m_x + v.m_x, u.m_y + v.m_y, u.m_z + v.m_z);
}

inline Vector3 operator- (const Vector3 &u, const Vector3 &v)
{
    return Vector3 (u.m_x - v.m_x, u.m_y - v.m_y, u.m_z - v.m_z);
}

inline Vector3 operator* (const Vector3 &u, const Vector3 &v)
{
    return Vector3 (u.m_x * v.m_x, u.m_y * v.m_y, u.m_z * v.m_z);
}

inline Vector3 operator* (double t, const Vector3 &v) { return Vector3 (t * v.m_x, t * v.m_y, t * v.m_z); }

inline Vector3 operator* (const Vector3 &v, double t) { return t * v; }

inline Vector3 operator/ (const Vector3 &v, double t) { return (1 / t) * v; }

double Dot (const Vector3 &u, const Vector3 &v);

inline Vector3 Reflect (const Vector3 &v, const Vector3 &n) { return v - (Dot (v, n) * n) * 2; }

inline Vector3 Refract (const Vector3 &uv, const Vector3 &n, double etai_over_etat)
{
    auto cos_theta = fmin (Dot (uv * -1, n), 1.0);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt (fabs (1.0 - r_out_perp.LengthSquared ())) * n;

    return r_out_perp + r_out_parallel;
}

inline Vector3 UnitVector (Vector3 v) { return v / v.Length (); }

inline Vector3 RandomInUnitDisk ()
{
    while (true)
    {
        auto p = Vector3 (RandomDouble (-1, 1), RandomDouble (-1, 1), 0);
        if (p.LengthSquared () < 1)
            return p;
    }
}

Vector3 Cross (const Vector3 &u, const Vector3 &v);

inline Vector3 RandomInUnitSphere ()
{
    while (true)
    {
        auto p = Vector3::RandomVector (-1, 1);
        if (p.LengthSquared () < 1)
            return p;
    }
}

inline Vector3 Vector3::RandomVector () { return Vector3 (RandomDouble (), RandomDouble (), RandomDouble ()); }

inline Vector3 Vector3::RandomVector (double min, double max)
{
    return Vector3 (RandomDouble (min, max), RandomDouble (min, max), RandomDouble (min, max));
}

inline Vector3 RandomUnitVector () { return UnitVector (RandomInUnitSphere ()); }

inline Vector3 RandomOnHemisphere (const Vector3 &normal)
{
    Vector3 on_unit_sphere = RandomUnitVector ();
    if (Dot (on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return on_unit_sphere * -1;
}

// Type Aliases for Vector3 class
using Point3 = Vector3;
using Color = Vector3;

#endif
