
#include "Renderer.h"
#include "Logger.h"

ID3D11Device *Renderer::GetDevice () const { return m_pd3dDevice; }

ID3D11DeviceContext *Renderer::GetDeviceContext () const { return m_pd3dDeviceContext; }

IDXGISwapChain *Renderer::GetSwapChain () const { return m_pSwapChain; }

bool Renderer::CreateDeviceD3D (HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory (&sd, sizeof (sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
    };
    HRESULT res = D3D11CreateDeviceAndSwapChain (nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel,
            &m_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain (nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
                featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel,
                &m_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTargetView ();
    return true;
}

void Renderer::DrawImage (const class Image &image)
{
    // Ensure the image's GPU texture is up-to-date
    if (image.IsDirty ())
        const_cast<Image &> (image).UpdateGPUTexture (m_pd3dDevice, m_pd3dDeviceContext);
}

void Renderer::CleanupDeviceD3D ()
{
    CleanupRenderTarget ();

    if (m_pSwapChain)
    {
        m_pSwapChain->Release ();
        m_pSwapChain = nullptr;
    }
    if (m_pd3dDeviceContext)
    {
        m_pd3dDeviceContext->Release ();
        m_pd3dDeviceContext = nullptr;
    }
    if (m_pd3dDevice)
    {
        m_pd3dDevice->Release ();
        m_pd3dDevice = nullptr;
    }
}

void Renderer::CleanupRenderTarget ()
{
    if (m_mainRenderTargetView)
    {
        m_mainRenderTargetView->Release ();
        m_mainRenderTargetView = nullptr;
    }
}

bool Renderer::CreateRenderTargetView ()
{
    ID3D11Texture2D *pBackBuffer;
    m_pSwapChain->GetBuffer (0, IID_PPV_ARGS (&pBackBuffer));
    m_pd3dDevice->CreateRenderTargetView (pBackBuffer, nullptr, &m_mainRenderTargetView);
    pBackBuffer->Release ();

    return true;
}

bool Renderer::Initialize (HWND hwnd, uint32_t width, uint32_t height)
{
    Logger::Log (Logger::Level::INFO, "Initializing DirectX 11 Renderer...");

    m_WindowHandle = hwnd;
    m_windowWidth = width;
    m_windowHeight = height;

    return CreateDeviceD3D (hwnd);
}
void Renderer::Shutdown ()
{
    Logger::Log (Logger::Level::INFO, "Shutting down DirectX 11 Renderer...");
    CleanupDeviceD3D ();
}

void Renderer::BeginFrame (const float clearColor[4])
{
    m_pd3dDeviceContext->OMSetRenderTargets (1, &m_mainRenderTargetView, nullptr);
    m_pd3dDeviceContext->ClearRenderTargetView (m_mainRenderTargetView, clearColor);
    // Optionally: set viewport here
}

void Renderer::EndFrame () { Present (); }

void Renderer::Present ()
{
    // Present the frame
    HRESULT hr = m_pSwapChain->Present (1, 0); // VSync enabled
    if (FAILED (hr))
    {
        Logger::Log (Logger::Level::ERR, "Failed to present frame");
    }
}

void Renderer::OnWindowResize (uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0)
        return;
    m_windowWidth = width;
    m_windowHeight = height;

    // Release render target view
    CleanupRenderTarget ();

    // Resize swap chain buffers
    HRESULT hr = m_pSwapChain->ResizeBuffers (0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED (hr))
    {
        Logger::Log (Logger::Level::ERR, "Failed to resize swap chain buffers");
        return;
    }

    // Recreate render target view
    CreateRenderTargetView ();
}
