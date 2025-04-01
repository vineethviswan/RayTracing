#pragma once

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstdint>
#include "Vector3.h"

struct PixelData
{
	Vector3 pixel_00_location;
	Vector3 pixel_delta_u;
	Vector3 pixel_delta_v;
};

class Camera
{
public:
	Camera () = default;
	PixelData Init(uint32_t width, uint32_t height);
	~Camera() {}

private:		
};

#endif // !CAMERA_H_