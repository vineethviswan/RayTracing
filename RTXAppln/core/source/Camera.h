#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Vector3.h"
#include "Utility.h"
#include "Hittable.h"

class Camera
{
public: 
    Camera () = default;
	Camera (uint32_t &width, uint32_t &height);
    void Initialize ();

    Ray GetRay (uint32_t i, uint32_t j) const;

    // RayColor now takes a scene (Hittable) so it can shade hits against the world.
    Color RayColor (const Ray &r, const Hittable &world);

private:
    uint32_t m_Width;
    uint32_t m_Height;
    Point3 camera_center; // Camera center
    Point3 pixel00_loc; // Location of pixel 0, 0
    Vector3 pixel_delta_u; // Offset to pixel to the right
    Vector3 pixel_delta_v; // Offset to pixel below
    Point3 lookfrom = Point3 (0, 0, 0); // Point camera is looking from

    Point3 lookat = Point3 (0, 0, -1); // Point camera is looking at
    Vector3 vup; // Camera-relative "up" direction
    Vector3 u;
    Vector3 v;
    Vector3 w; // Camera frame basis vectors
};

#endif // CAMERA_H
