/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished :
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include "AppLayer.h"
#include "Application.h"
#include "Logger.h"
#include "ImGuiLayer.h"
#include "Image.h"
#include "Constants.h"
#include <memory>

int main ()
{
    Logger::Log (Logger::Level::INFO, "Ray Tracing !!!");

    AppSpecification appSpec;
    appSpec.Name = L"RayTracer";
    appSpec.WindowSpec.Width = WINDOW_WIDTH;
    appSpec.WindowSpec.Height = WINDOW_HEIGHT;

    Application app (appSpec);

    auto sharedImage = std::make_shared<Image> (IMAGE_WIDTH, IMAGE_HEIGHT);

    // Create layers locally so we can wire the enqueue callback
    auto imguiLayerUP = std::make_unique<ImGuiLayer> (sharedImage);
    auto appLayerUP = std::make_unique<AppLayer> (sharedImage);

    ImGuiLayer *imguiPtr = imguiLayerUP.get ();
    AppLayer *appPtr = appLayerUP.get ();
    imguiPtr->SetEnqueueRenderCallback ([appPtr]() {
        appPtr->EnqueueRenderJob ();
    });
    imguiPtr->m_GetLastRenderTime = [appPtr]() {
        return appPtr->GetLastRenderTimeMs(); };

    app.PushLayer (std::move (imguiLayerUP));
    app.PushLayer (std::move (appLayerUP));
    app.Run ();

    return 0;
}
