#pragma once

#include <memory>
#include <vector>

#include "Layer.h"

class Renderer;

class Application
{
public:
	Application ();
	~Application ();

	static Application* GetInstance () { return s_Instance; }
	void Run ();
	void PushLayer (Layer* layer);
	Renderer& GetRenderer () { return *m_Renderer; }

private:
	static Application* s_Instance;
	std::vector<Layer> m_LayerStack;
	std::unique_ptr<Renderer> m_Renderer;
	bool m_Running;
};