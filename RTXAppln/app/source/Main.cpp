
/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished :
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include "AppLayer.h"
#include "Application.h"
#include "ImGuiLayer.h"
#include "Logger.h"

int main ()
{
    Logger::Log (Logger::Level::INFO, "Ray Tracing !!!");

    AppSpecification appSpec;
    appSpec.Name = L"RayTracer";
    appSpec.WindowSpec.Width = 1309;
    appSpec.WindowSpec.Height = 605;

    Application app (appSpec);

    // Create a single shared Image and share it between layers
    auto sharedImage = std::make_shared<Image> (960, 560);

    // Create layers locally so we can wire the enqueue callback
    auto imguiLayerUP = std::make_unique<ImGuiLayer> (sharedImage);
    auto appLayerUP = std::make_unique<AppLayer> (sharedImage);

    // Wire enqueue callback: ImGui button should enqueue a heavy render job
    ImGuiLayer *imguiPtr = imguiLayerUP.get ();
    AppLayer *appPtr = appLayerUP.get ();
    imguiPtr->SetEnqueueRenderCallback ([appPtr] () { appPtr->EnqueueRenderJob (); });

    app.PushLayer (std::move (imguiLayerUP));
    app.PushLayer (std::move (appLayerUP));
    app.Run ();

    return 0;
}
