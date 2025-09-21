
/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished : 
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include "Application.h"
#include "Layer.h"
#include "Logger.h"
 
class TestLayer : public Layer
{
    void OnUpdate (float deltaTime) override
    {
        // Log frame time, input handling
    }

    void OnRender () override
    {
        // Draw test triangle        
    }
};

// Main function test
int main ()
{
    Logger::Log (Logger::Level::INFO, "Ray Tracing!!");

    Application app;    
    app.Run ();
}