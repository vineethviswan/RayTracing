#pragma once

#include <functional>
#include <imgui.h>
#include <memory>
#include "Image.h"
#include "Constants.h"

#include "Renderer.h"
#include "Layer.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer (std::shared_ptr<Image> image);
    ~ImGuiLayer () override = default;

    void Initialize (HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *deviceContext);
    void Shutdown ();
    void BeginFrame ();
    void EndFrame ();
    void ProcessEvent (UINT msg, WPARAM wParam, LPARAM lParam);

    // Enqueue a render job (UI -> producer). This should be a lightweight call.
    void SetEnqueueRenderCallback (std::function<void ()> cb) { m_EnqueueRender = std::move (cb); }
    std::function<double ()> m_GetLastRenderTime;

    void OnUpdate (double ts) override;
    void OnRender () override;
    void OnAttach () override { }

    void SetImage (std::shared_ptr<Image> image) { m_Image = std::move (image); }

private:
    std::shared_ptr<Image> m_Image = nullptr;
    std::function<void ()> m_EnqueueRender;
    uint32_t m_ViewportWidth = VIEWPORT_WIDTH;
    uint32_t m_ViewportHeight = VIEWPORT_HEIGHT;
};
