
#include <algorithm>
#include <assert.h>

#include "Application.h"
#include "Logger.h"

static Application *s_Application = nullptr;

Application::Application (const AppSpecification &spec) : m_Specification (spec)
{
    Logger::Log (Logger::Level::INFO, "Creating Application.");

    // Set window title to app name if empty
    if (m_Specification.WindowSpec.Title == L"")
        m_Specification.WindowSpec.Title = m_Specification.Name;

    s_Application = this;
    m_Window = std::make_shared<Window> (m_Specification.WindowSpec);
    m_Window->Create ();
}

Application::~Application ()
{
    m_Window->Destroy ();
    s_Application = nullptr;
}

void Application::Run ()
{
    m_Running = true;
    double lastTime = GetTime ();

    while (m_Running)
    {
        if (m_Window->ShouldClose ())
        {
            Stop ();
            break;
        }

        double currentTime = GetTime ();
        double timestep = std::clamp (currentTime - lastTime, 0.001, 0.1);
        lastTime = currentTime;

        // Main layer update here
        for (const std::unique_ptr<Layer> &layer: m_LayerStack)
            layer->OnUpdate (timestep);

        // NOTE: rendering can be done elsewhere (eg. render thread)
        for (const std::unique_ptr<Layer> &layer: m_LayerStack)
            layer->OnRender ();

        m_Window->Update ();
    }
}

double Application::GetTime ()
{
    static LARGE_INTEGER frequency;
    static BOOL use_qpc = QueryPerformanceFrequency (&frequency);
    LARGE_INTEGER now;
    QueryPerformanceCounter (&now);
    return static_cast<double> (now.QuadPart) / frequency.QuadPart;
}

void Application::Stop () { m_Running = false; }

Application &Application::Get ()
{
    assert (s_Application);
    return *s_Application;
}
