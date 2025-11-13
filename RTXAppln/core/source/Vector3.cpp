
#include "Vector3.h"

Vector3 &Vector3::operator*= (const double t)
{

    m_x *= t;
    m_y *= t;
    m_z *= t;
    return *this;
}

Vector3 &Vector3::operator+= (const Vector3 &vec)
{
    m_x += vec.m_x;
    m_y += vec.m_y;
    m_z += vec.m_z;
    return *this;
}

Vector3 &Vector3::operator/= (double t) { return *this *= (1 / t); }

double Vector3::Length () const { return sqrt (m_x * m_x + m_y * m_y + m_z * m_z); }

double Vector3::LengthSquared () const { return m_x * m_x + m_y * m_y + m_z * m_z; }

bool Vector3::NearZero () const
{
    // Return true if the vector is close to zero in all dimensions.
    auto s = 1e-8;
    return (fabs (m_x) < s) && (fabs (m_y) < s) && (fabs (m_z) < s);
}


double Dot (const Vector3 &u, const Vector3 &v)
{
    return u.GetX () * v.GetX () + u.GetY () * v.GetY () + u.GetZ () * v.GetZ ();
}

Vector3 Cross (const Vector3 &u, const Vector3 &v)
{
    return Vector3 (u.GetY () * v.GetZ () - u.GetZ () * v.GetY (), u.GetZ () * v.GetX () - u.GetX () * v.GetZ (),
            u.GetX () * v.GetY () - u.GetY () * v.GetX ());
}
