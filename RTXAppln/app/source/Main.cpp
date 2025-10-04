
/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished :
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include "AppLayer.h"
#include "Application.h"
#include "Logger.h"

int main ()
{
    Logger::Log (Logger::Level::INFO, "Ray Tracing !!!");

    AppSpecification appSpec;
    appSpec.Name = L"RayTracer";
    appSpec.WindowSpec.Width = 1309;
    appSpec.WindowSpec.Height = 605;

    Application app (appSpec);
    app.PushLayer (std::make_unique<AppLayer> ());
    app.Run ();

    return 0;
}
