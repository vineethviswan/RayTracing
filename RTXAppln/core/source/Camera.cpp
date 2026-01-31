
#include "Camera.h"
#include <cmath>

Camera::Camera (uint32_t &width, uint32_t &height) 
	: m_Width (width), m_Height (height)
{ 	
	Initialize (); 
}

void Camera::Initialize ()
{ 
	lookfrom = Point3 (0, 0, 0);
    lookat = Point3 (0, 0, 0);
    camera_center = Point3 (0, 0, 0);
    
    double focal_length = 1.0; 
	auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double> (m_Width) / m_Height);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vector3 (viewport_width, 0, 0);
    auto viewport_v = Vector3 (0, -viewport_height, 0);

    pixel_samples_scale = 1.0 / samples_per_pixel;

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / m_Width;
    pixel_delta_v = viewport_v / m_Height;

	// Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - Vector3 (0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

Ray Camera::GetRay (uint32_t i, uint32_t j) const
{ 
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j.

    auto offset = SampleSquare ();
    auto pixel_sample = pixel00_loc + ((i + offset.GetX ()) * pixel_delta_u) + ((j + offset.GetY ()) * pixel_delta_v);

    auto ray_origin = camera_center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray (ray_origin, ray_direction);        
}

Color Camera::RayColor (const Ray &r, const Hittable &world)
{
    HitRecord rec;
    if (world.Hit (r, Interval (0, INFNTY), rec))
    {
        return 0.5 * (rec.normal + Color (1, 1, 1));
    }

    Vector3 unit_direction = UnitVector (r.direction ());
    auto a = 0.5 * (unit_direction.GetY () + 1.0);
    return (1.0 - a) * Color (1.0, 1.0, 1.0) + a * Color (0.5, 0.7, 1.0);
}

Vector3 Camera::SampleSquare () const
{
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return Vector3 (RandomDouble () - 0.5, RandomDouble () - 0.5, 0);
}
