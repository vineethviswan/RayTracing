
#include "Camera.h"

PixelData Camera::Init(uint32_t width, uint32_t height)
{
	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * (double(width) / height);
	auto camera_center = Point3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = Vector3(viewport_width, 0, 0);
	auto viewport_v = Vector3(0, -1 * viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / width;
	auto pixel_delta_v = viewport_v / height;

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = camera_center
		- Vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	return PixelData{ pixel00_loc, pixel_delta_u, pixel_delta_v };

}