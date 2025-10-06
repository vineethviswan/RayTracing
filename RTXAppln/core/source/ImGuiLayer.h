#pragma once

#include <imgui.h>

#include "Renderer.h"
#include "Layer.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer () = default;
	~ImGuiLayer () override = default;
	
	void Initialize (HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *deviceContext);
    void Shutdown ();    
    void BeginFrame ();
    void EndFrame ();
    void ProcessEvent (UINT msg, WPARAM wParam, LPARAM lParam);

	void OnUpdate (double ts) override;
    void OnRender () override;

private:
};
