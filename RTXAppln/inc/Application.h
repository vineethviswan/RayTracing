#pragma once

#include <d3d11.h>
#include <cstdint>
#include <memory>
#include "Layer.h"

class Application
{
public:
	
	Application(const wchar_t* name, uint32_t width, uint32_t height);
	~Application();
	
	void Init();
	void Shutdown();
	void Run();
	void Close();
	HWND GetWindowHandle() const { return m_WindowHandle; }
		
	void UpdateWindowSize(uint32_t width, uint32_t height);
	void ResizeWindow();

	void SetLayer(std::shared_ptr<Layer> layer);

private:

	//DirectX11 
	bool CreateDevice3D();
	void CleanupDevice3D();
	void CreateRenderTargetView();
	void CleanupRenderTargetView();
	void ClearRenderTargetView();

	bool IsWindowResized() const;

private:
	
	//Window
	HWND m_WindowHandle;
	WNDCLASSEX m_WndClass;  

	//Application
	const wchar_t* m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
	bool m_Running = false;
	uint32_t m_ResizeHeight;
	uint32_t m_ResizeWidth;	

	// Layers
	std::shared_ptr<Layer> m_Layer;

	ID3D11ShaderResourceView* m_ShaderView = nullptr;
};