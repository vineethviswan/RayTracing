
#include "ImGuiLayer.h"
#include "Logger.h"

ImGuiLayer::ImGuiLayer (std::shared_ptr<Image> image) :
    m_ViewportWidth (960), m_ViewportHeight (559), m_Image (std::move (image))
{
}

void ImGuiLayer::Initialize (HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
    Logger::Log (Logger::Level::INFO, "Initializing ImGui.");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION ();
    ImGui::CreateContext ();
    ImGuiIO &io = ImGui::GetIO ();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui::StyleColorsDark ();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init (hwnd);
    ImGui_ImplDX11_Init (device, deviceContext);
}

void ImGuiLayer::Shutdown ()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown ();
    ImGui_ImplWin32_Shutdown ();
    ImGui::DestroyContext ();
}

void ImGuiLayer::BeginFrame ()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame ();
    ImGui_ImplWin32_NewFrame ();
    ImGui::NewFrame ();
}

void ImGuiLayer::EndFrame ()
{
    ImGui::Render ();
    ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ());
}

void ImGuiLayer::ProcessEvent (UINT msg, WPARAM wParam, LPARAM lParam) { }

void ImGuiLayer::OnUpdate (double ts) { }

void ImGuiLayer::OnRender ()
{
    ImGui::SetNextWindowSize (ImVec2 (320, 559));
    ImGui::Begin ("Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::NewLine ();
    ImGui::Text ("Last render: %.3f ms", 5.2556f);
    ImGui::Separator ();
    if (ImGui::Button ("Render"))
    {
        Logger::Log (Logger::Level::INFO, "ImGui: Render button pressed");
        // enqueue a render job on worker thread (fast non-blocking)
        if (m_EnqueueRender)
            m_EnqueueRender ();
    }
    ImGui::End ();

    ImGui::SetNextWindowSize (ImVec2 (m_ViewportWidth, m_ViewportHeight));
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
    ImGui::Begin ("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (m_Image && m_Image->GetSRV ())
    {
        ImGui::Image (
                (void *) m_Image->GetSRV (), ImVec2 ((float) m_Image->GetWidth (), (float) m_Image->GetHeight ()));
    }

    ImGui::End ();
    ImGui::PopStyleVar ();
}
