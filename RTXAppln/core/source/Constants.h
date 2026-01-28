#pragma once

#ifndef CONSTANTS
#define CONSTANTS

constexpr uint32_t WINDOW_WIDTH  = 1309;
constexpr uint32_t WINDOW_HEIGHT = 605;

// Image / render target pixel resolution (used by Image, ImGui display, etc)
constexpr uint32_t IMAGE_WIDTH   = 960;
constexpr uint32_t IMAGE_HEIGHT  = 559;

// Viewport pixel size used by ImGui display (kept in sync with image)
constexpr uint32_t VIEWPORT_WIDTH  = IMAGE_WIDTH;
constexpr uint32_t VIEWPORT_HEIGHT = IMAGE_HEIGHT; // Use IMAGE_HEIGHT for ImGui viewport

// Derived values
constexpr double ASPECT_RATIO = 16.0 / 9.0;

constexpr int CHANNELS = 4;

const double INFNTY = std::numeric_limits<double>::infinity ();

#endif // !CONSTANTS
