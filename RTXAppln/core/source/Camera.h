#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Vector3.h"
#include "Utility.h"
#include "Hittable.h"
#include "Material.h"

class Camera
{
public: 
    Camera () = default;
	Camera (uint32_t &width, uint32_t &height);
    void Initialize ();

    Ray GetRay (uint32_t i, uint32_t j) const;

    // RayColor now takes a scene (Hittable) so it can shade hits against the world.
    Color RayColor (const Ray &r, int depth, const Hittable &world);

    int GetSamplesPerPixel () const { return samples_per_pixel; }
    double GetPixelSamplesScale () const { return pixel_samples_scale; }
    int GetMaxDepth () const { return max_depth; }

    Vector3 SampleSquare () const;
    Point3 DefocusDiskSample () const;

private:
    uint32_t m_Width;
    uint32_t m_Height;

    Point3 camera_center; // Camera center
    Point3 pixel00_loc; // Location of pixel 0, 0
    Point3 lookfrom = Point3 (0, 0, 0); // Point camera is looking from
    Point3 lookat = Point3 (0, 0, -1); // Point camera is looking at

    Vector3 pixel_delta_u; // Offset to pixel to the right
    Vector3 pixel_delta_v; // Offset to pixel below    
    Vector3 vup; // Camera-relative "up" direction
    Vector3 u, v, w; // Camera frame basis vectors

    int samples_per_pixel; // Count of random samples for each pixel
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    int max_depth; // Maximum number of ray bounces into scene

    double vfov; // Vertical view angle (field of view)
    double defocus_angle; // Variation angle of rays through each pixel
    double focus_dist; // Distance from camera lookfrom point to plane of perfect focus
    Vector3 defocus_disk_u; // Defocus disk horizontal radius
    Vector3 defocus_disk_v; // Defocus disk vertical radius
};

#endif // CAMERA_H
