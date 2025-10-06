
#include "ImGuiLayer.h"
#include "Logger.h"

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
    ImGui::End ();

    ImGui::SetNextWindowSize (ImVec2 (960, 559));
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
    ImGui::Begin ("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::End ();
    ImGui::PopStyleVar ();
}
