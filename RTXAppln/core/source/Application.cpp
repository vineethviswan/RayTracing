
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
    if (!m_Renderer.Initialize (
                m_Window->GetHandle (), m_Specification.WindowSpec.Width, m_Specification.WindowSpec.Height))
    {
        Logger::Log (Logger::Level::ERR, "Failed to initialize Renderer.");

        m_Renderer.CleanupDeviceD3D ();
        m_Window->Destroy ();
    }
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

    // Define a clear color (RGBA)
    const float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};

    while (m_Running)
    {
        m_Window->Update ();
        if (m_Window->ShouldClose ())
        {
            Stop ();
            break;
        }

        double currentTime = GetTime ();
        double timestep = std::clamp (currentTime - lastTime, 0.001, 0.1);
        lastTime = currentTime;

        // Start rendering for this frame
        m_Renderer.BeginFrame (clearColor);

        // Main layer update here
        for (const std::unique_ptr<Layer> &layer: m_LayerStack)
            layer->OnUpdate (timestep);

        // NOTE: rendering can be done elsewhere (eg. render thread)
        for (const std::unique_ptr<Layer> &layer: m_LayerStack)
            layer->OnRender ();

        // End rendering for this frame (present)
        m_Renderer.EndFrame ();

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
