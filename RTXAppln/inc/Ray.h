#pragma once

#ifndef _RAY_H
#define _RAY_H

#include "Vector3.h"

using Color = Vector3;

class Ray {
public:
	Ray() {}
	Ray(const Point3& origin, const Vector3& direction) : m_origin(origin), m_direction(direction) {}

	Point3 GetOrigin() const {
		return m_origin;
	}

	Vector3 GetDirection() const {
		return m_direction;
	}

	//P(t) = origin + t * direction
	Point3 At(const double t) const {
		return m_origin + (m_direction * t);
	}

private:
	Point3 m_origin;
	Vector3 m_direction;
};

Color RayColor(const Ray& r) {
	Vector3 unit_direction = UnitVector(r.GetDirection());
	auto a = 0.5 * (unit_direction.GetY() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

#endif // !_RAY_H
