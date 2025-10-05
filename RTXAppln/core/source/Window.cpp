
#include "Window.h"
#include "Logger.h"

LRESULT WINAPI WndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window (const WindowSpecification &specification) : m_Specification (specification) { }

Window::~Window () { Destroy (); }

bool Window::ShouldClose () const { return m_ShouldClose; }

void Window::Create ()
{
    Logger::Log (Logger::Level::INFO, "Creating Window.");

    // Create window
    m_WndClass = {sizeof (m_WndClass), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle (nullptr), nullptr, nullptr,
            nullptr, nullptr, L"WindowClass", nullptr};

    ::RegisterClassExW (&m_WndClass);
    m_WindowHandle = ::CreateWindowW (m_WndClass.lpszClassName, m_Specification.Title, WS_OVERLAPPEDWINDOW, 200, 200,
            m_Specification.Width, m_Specification.Height, nullptr, nullptr, m_WndClass.hInstance, nullptr);

    SetWindowLongPtr (m_WindowHandle, GWLP_USERDATA, (LONG_PTR) this);

    // Show Window
    ::ShowWindow (m_WindowHandle, SW_SHOWDEFAULT);
    ::UpdateWindow (m_WindowHandle);
}

void Window::Destroy ()
{
    if (m_WindowHandle)
        ::DestroyWindow (m_WindowHandle);
    ::UnregisterClassW (m_WndClass.lpszClassName, m_WndClass.hInstance);
}

void Window::Update ()
{
    MSG msg;
    while (::PeekMessage (&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage (&msg);
        ::DispatchMessage (&msg);
    }
}

bool Window::OnSizeChanged (UINT width, UINT height) { return true; }

LRESULT WINAPI WndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    Window *winptr = (Window *) GetWindowLongPtr (hWnd, GWLP_USERDATA);

    switch (msg)
    {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;

            if (winptr != NULL)
            {
                winptr->OnSizeChanged ((UINT) LOWORD (lParam), (UINT) HIWORD (lParam));
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            if (winptr != nullptr)
                winptr->m_ShouldClose = true;
            ::PostQuitMessage (0);
            return 0;
    }
    return ::DefWindowProcW (hWnd, msg, wParam, lParam);
}
