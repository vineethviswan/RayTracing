#pragma once

#include "Layer.h"

class LayerStack
{
public:
	void PushLayer (Layer* layer);
	void Update (float deltaTime);
	void Render ();
};