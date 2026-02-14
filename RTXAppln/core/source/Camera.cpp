
#include "Camera.h"
#include "Hittable.h"
#include <cmath>

Camera::Camera (uint32_t &width, uint32_t &height) 
	: m_Width (width), m_Height (height)
{ 	
	Initialize (); 
}

void Camera::Initialize ()
{ 
	lookfrom = Point3 (-2, 2, 1);
    lookat = Point3 (0, 0, -1);
    camera_center = lookfrom;
    vfov = 20; // Vertical view angle (field of view)
    vup = Vector3 (0, 1, 0); // Camera-relative "up" direction
    
    double focal_length = (lookfrom - lookat).Length (); 
	auto theta = DegreesToRadian (vfov);
    auto h = std::tan (theta / 2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (static_cast<double> (m_Width) / m_Height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = UnitVector (lookfrom - lookat);
    u = UnitVector (Cross (vup, w));
    v = Cross (w, u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge
    auto viewport_v = viewport_height * (v * -1); // Vector down viewport vertical edge

    samples_per_pixel = 10;
    pixel_samples_scale = 1.0 / samples_per_pixel;
    max_depth = 50;

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / m_Width;
    pixel_delta_v = viewport_v / m_Height;

	// Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
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

Color Camera::RayColor (const Ray &r, int depth, const Hittable &world)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color (0, 0, 0);

    HitRecord rec;
    if (world.Hit (r, Interval (0.001, INFNTY), rec))
    {       
        /*Vector3 direction = rec.normal + RandomUnitVector();
        return 0.1 * RayColor (Ray (rec.p, direction), depth - 1, world); */               

        Ray scattered;
        Color attenuation;
        if (rec.mat->Scatter (r, rec, attenuation, scattered))
            return attenuation * RayColor (scattered, depth - 1, world);
        return Color (0, 0, 0);
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
