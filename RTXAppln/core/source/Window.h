#pragma once

#include <Windows.h>
#include <string>

struct WindowSpecification
{
    LPCWSTR Title = L"Ray Tracing";
    uint32_t Width = 1309;
    uint32_t Height = 605;
    bool IsResizable = true;
};

class Window
{
public:
    Window (const WindowSpecification &specification = WindowSpecification ());
    ~Window ();

    void Create ();
    void Destroy ();
    void Update ();
    bool OnSizeChanged (UINT width, UINT height);

    bool ShouldClose () const;
    HWND GetHandle () const { return m_WindowHandle; }

private:
    WindowSpecification m_Specification;
    HWND m_WindowHandle;
    WNDCLASSEXW m_WndClass;
};
