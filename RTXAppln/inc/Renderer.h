#pragma once

#include <d3d11.h>

class Renderer
{
    bool Initialize (HWND hwnd); // Device, context, swap chain
    void BeginFrame ();          // Clear render target
    void EndFrame ();            // Present
    void DrawTestTriangle ();    // Verify DirectX works
};
