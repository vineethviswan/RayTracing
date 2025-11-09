#include "AppLayer.h"
#include "Application.h"
#include "Logger.h"
#include "Constants.h"

#include <algorithm>
#include <chrono>

AppLayer::AppLayer (std::shared_ptr<Image> image)
{
    Logger::Log (Logger::Level::INFO, "Creating AppLayer (shared image)");

    // Initialize front/back images
    if (image)
    {
        m_FrontImage = image;
        m_BackImage = std::make_shared<Image> (IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else
    {
        m_FrontImage = std::make_shared<Image> (IMAGE_WIDTH, IMAGE_HEIGHT);
        m_BackImage = std::make_shared<Image> (IMAGE_WIDTH, IMAGE_HEIGHT);
    }

    // start worker
    m_WorkerRunning = true;
    m_Worker = std::thread (
            [this] ()
            {
                while (m_WorkerRunning)
                {
                    auto job = m_CommandQueue.PopBlocking ();
                    if (!job)
                        continue;
                    job ();
                }
            });
}

AppLayer::~AppLayer ()
{
    m_WorkerRunning = false;
    m_CommandQueue.Shutdown ();
    Logger::Log (Logger::Level::INFO, "AppLayer: shutting down worker");
    if (m_Worker.joinable ())
        m_Worker.join ();
}

void AppLayer::OnAttach ()
{
    Logger::Log(Logger::Level::INFO, "AppLayer attached with image size "
        + std::to_string(m_FrontImage->GetWidth()) + " x "
        + std::to_string(m_FrontImage->GetHeight()));
}

// Utility function to pack r, g, b, a (all in [0,1]) into a uint32_t RGBA value
uint32_t AppLayer::PackColor (double r, double g, double b, double a)
{
    uint8_t R = static_cast<uint8_t> (std::clamp (r, 0.0, 1.0) * 255.0);
    uint8_t G = static_cast<uint8_t> (std::clamp (g, 0.0, 1.0) * 255.0);
    uint8_t B = static_cast<uint8_t> (std::clamp (b, 0.0, 1.0) * 255.0);
    uint8_t A = static_cast<uint8_t> (std::clamp (a, 0.0, 1.0) * 255.0);
    return (A << 24) | (B << 16) | (G << 8) | R;
}

void AppLayer::GenerateTestPattern (Image &target)
{
    const uint32_t width = target.GetWidth ();
    const uint32_t height = target.GetHeight ();

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            auto r = double (x) / (double) width;
            auto g = double (y) / (double) height;
            auto b = 0.0;

            auto color = PackColor (r, g, b, 1.0);
            target.SetPixel (x, y, color);
        }
    }
}

void AppLayer::OnUpdate (double ts) { }

void AppLayer::OnRender ()
{
    auto &renderer = Application::Get ().GetRenderer ();
    // Swap front/back if worker produced a new image
    if (m_BackReady.load (std::memory_order_acquire))
    {
        std::lock_guard<std::mutex> lk (m_SwapMutex);
        m_FrontImage.swap (m_BackImage);
        m_BackReady.store (false, std::memory_order_release);

        // Upload new front image to GPU on render thread
        m_FrontImage->UpdateGPUTexture (renderer.GetDevice (), renderer.GetDeviceContext ());
    }

    // Draw front image if available
    if (m_FrontImage && m_FrontImage->GetSRV ())
        renderer.DrawImage (*m_FrontImage);
}

void AppLayer::EnqueueRenderJob ()
{
    Logger::Log(Logger::Level::INFO, "AppLayer: enqueueing render job");
    m_CommandQueue.Push([this]()
    {
        if (m_BackImage)
        {
            // First time: no image showing yet, generate directly into front buffer
            if (!m_FrontImage->GetSRV())
            {
                Logger::Log(Logger::Level::INFO, "First render - generating into front buffer");
                GenerateTestPattern(*m_FrontImage);
                m_FrontImage->UpdateGPUTexture(Application::Get().GetRenderer().GetDevice(),
                                               Application::Get().GetRenderer().GetDeviceContext());
                m_BackReady.store(false, std::memory_order_release);  // Don't swap
            }
            else
            {
                // Normal case: generate into back buffer for swap
                Logger::Log(Logger::Level::INFO, "Generating into back buffer");
                GenerateTestPattern(*m_BackImage);
                m_BackReady.store(true, std::memory_order_release);  // Signal for swap
            }
        }
    });
}

