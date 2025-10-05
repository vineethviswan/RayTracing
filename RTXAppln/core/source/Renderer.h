#pragma once

#include <d3d11.h>
#include <string>

class Renderer
{
public:
    Renderer () = default;

    ID3D11Device *GetDevice () const;
    ID3D11DeviceContext *GetDeviceContext () const;
    IDXGISwapChain *GetSwapChain () const;

    bool Initialize (HWND hwnd, uint32_t width, uint32_t height);
    void Shutdown ();
    void BeginFrame (const float clearColor[4]);
    void EndFrame ();
    void Present ();
    void OnWindowResize (uint32_t width, uint32_t height);

    bool CreateDeviceD3D (HWND hWnd);
    void CleanupDeviceD3D ();
    bool CreateRenderTargetView ();
    void CleanupRenderTarget ();

private:
    ID3D11Device *m_pd3dDevice = nullptr;
    ID3D11DeviceContext *m_pd3dDeviceContext = nullptr;
    IDXGISwapChain *m_pSwapChain = nullptr;
    ID3D11RenderTargetView *m_mainRenderTargetView = nullptr;

    uint32_t m_windowWidth = 0;
    uint32_t m_windowHeight = 0;
    uint32_t g_ResizeWidth = 0;
    uint32_t g_ResizeHeight = 0;
    HWND m_WindowHandle;
};
