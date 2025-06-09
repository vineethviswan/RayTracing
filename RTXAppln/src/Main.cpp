
/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished : 
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include <iostream>

#include "Application.h"
#include "RayTracingLayer.h"
#include "UILayer.h"

int main()
{
	std::cout << "RAY TRACING !!!\n";
    
    // Initialize application
    Application app;

    // Create layers
    auto rayTracingLayer = std::make_unique<RayTracingLayer> ();
    auto uiLayer = std::make_unique<UILayer> ();

    app.PushLayer (rayTracingLayer.get ());
    app.PushLayer (uiLayer.get ());

    // Run application
    app.Run ();
    
	return 0;
}