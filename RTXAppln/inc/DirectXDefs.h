#pragma once

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
