#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ImGuiLayer.h"
#include "Layer.h"
#include "Renderer.h"
#include "Window.h"

struct AppSpecification
{
    LPCWSTR Name = L"Ray Tracing";
    WindowSpecification WindowSpec;
};

class Application
{
public:
    Application (const AppSpecification &spec);
    virtual ~Application ();

    void Run ();
    void Stop ();

    void PushLayer (std::unique_ptr<Layer> &&layer);
    Renderer &GetRenderer () { return m_Renderer; }
    static Application &Get ();
    static double GetTime ();
    ImGuiLayer *GetImGuiLayer ();

private:
    AppSpecification m_Specification;
    bool m_Running = false;
    std::shared_ptr<Window> m_Window;
    Renderer m_Renderer;
    ImGuiLayer *m_ImGuiLayer;

    std::vector<std::unique_ptr<Layer>> m_LayerStack;
};
