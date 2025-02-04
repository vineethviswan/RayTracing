
#include "Application.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include <string>

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Application::Application(const wchar_t* name, uint32_t width, uint32_t height) :
    m_Name(name), m_Width(width), m_Height(height), m_WindowHandle(0), m_WndClass()
{
    Init();    
}

void Application::Init()
{
    //Create window
    m_WndClass = { sizeof(m_WndClass), CS_CLASSDC, WndProc,
        0L, 0L, GetModuleHandle(nullptr), nullptr,
        nullptr, nullptr, nullptr, L"WindowClass", nullptr };

    ::RegisterClassExW(&m_WndClass);
    m_WindowHandle = ::CreateWindowW(m_WndClass.lpszClassName, m_Name, WS_OVERLAPPEDWINDOW,
        200, 200, m_Width, m_Height, nullptr, nullptr, m_WndClass.hInstance, nullptr);

    SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, (LONG_PTR)this);

    //Initialize Direct3D
    if (!CreateDevice3D())
    {
        CleanupDevice3D();
        ::UnregisterClassW(m_WndClass.lpszClassName, m_WndClass.hInstance);
        return;
    }

    //Show Window
    ::ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
    ::UpdateWindow(m_WindowHandle);

    //ImGui Initialization
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls     

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_WindowHandle);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
  
}

ID3D11Device* Application::GetDevice()
{
    return g_pd3dDevice;
}

bool Application::CreateDevice3D()
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_WindowHandle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTargetView();
	return true;
}

void Application::CleanupDevice3D()
{
    CleanupRenderTargetView();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void Application::CreateRenderTargetView()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void Application::CleanupRenderTargetView()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void Application::ClearRenderTargetView()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
}

void Application::Run()
{
    m_Running = true;
    
    while (m_Running)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                m_Running = false;
                break;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
                
        // ====================================================================================================
        
        ImGui::SetNextWindowSize(ImVec2(320, 559));
        if (ImGui::Begin("Settings", nullptr/*, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */))
        {
            if (ImGui::Button("Render"))
            {

            }            
        }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(960, 559));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport", nullptr/*, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove*/);

        //ImGui::Text(" Viewport: %dx%d", (ImGui::GetContentRegionAvail().x), (ImGui::GetContentRegionAvail().y));
        
        ImGui::End();
        ImGui::PopStyleVar();

        // ====================================================================================================

        // Rendering
        ImGui::Render();
        ClearRenderTargetView();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0);
    }
}

void Application::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDevice3D();

    if (m_WindowHandle)
        ::DestroyWindow(m_WindowHandle);
    ::UnregisterClassW(m_WndClass.lpszClassName, m_WndClass.hInstance);
}

void Application::Close()
{
    m_Running = false;
}

Application::~Application()
{   
    Shutdown();
}

void Application::ResizeWindow()
{
	CleanupRenderTargetView();
	g_pSwapChain->ResizeBuffers(0, m_ResizeWidth, m_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
	CreateRenderTargetView();
    m_Height = m_ResizeHeight;
    m_Width = m_ResizeWidth;
    m_ResizeWidth = 0; m_ResizeHeight = 0;	
}

void Application::UpdateWindowSize(uint32_t width, uint32_t height)
{
    if (width != 0 && height != 0)
    {
        m_ResizeHeight = height;
        m_ResizeWidth = width;        
    }
}

bool Application::IsWindowResized() const
{
    return (m_Height != m_ResizeHeight || m_Width != m_ResizeWidth);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Application* winptr = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;

        if (winptr != NULL)
        {
            winptr->UpdateWindowSize((uint32_t)LOWORD(lParam), (uint32_t)HIWORD(lParam));
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
