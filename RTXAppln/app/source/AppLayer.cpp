#include "AppLayer.h"
#include "Application.h"
#include "Logger.h"
#include "Constants.h"
#include "../core/source/HittableList.h"
#include "../core/source/Sphere.h"

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

    // create camera using image dimensions
    uint32_t w = m_FrontImage->GetWidth ();
    uint32_t h = m_FrontImage->GetHeight ();
    m_Camera = std::make_unique<Camera> (w, h);

    // create scene with two spheres
    /*m_World.add (make_shared<sphere> (Point3 (0, 0, -1), 0.5));
    m_World.add (make_shared<sphere> (Point3 (0, -100.5, -1), 100));*/

    auto material_ground = make_shared<Lambertian> (Color (0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian> (Color (0.1, 0.2, 0.5));
    //auto material_left = make_shared<Metal> (Color (0.8, 0.8, 0.8));
    //auto material_left = make_shared<Dielectric> (1.00 / 1.33);
    auto material_left = make_shared<Dielectric> (1.50);
    auto material_bubble = make_shared<Dielectric> (1.00 / 1.50);
    auto material_right = make_shared<Metal> (Color (0.8, 0.6, 0.2));

    m_World.add (make_shared<sphere> (Point3 (0.0, -100.5, -1.0), 100.0, material_ground));
    m_World.add (make_shared<sphere> (Point3 (0.0, 0.0, -1.2), 0.5, material_center));
    m_World.add (make_shared<sphere> (Point3 (-1.0, 0.0, -1.0), 0.5, material_left));
    m_World.add (make_shared<sphere> (Point3 (-1.0, 0.0, -1.0), 0.4, material_bubble));
    m_World.add (make_shared<sphere> (Point3 (1.0, 0.0, -1.0), 0.5, material_right));

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
    // Apply a linear to gamma transform for gamma 2
    r = LinearToGamma (r);
    g = LinearToGamma (g);
    b = LinearToGamma (b);
    
    uint8_t R = static_cast<uint8_t> (std::clamp (r, 0.0, 0.999) * 256.0);
    uint8_t G = static_cast<uint8_t> (std::clamp (g, 0.0, 0.999) * 256.0);
    uint8_t B = static_cast<uint8_t> (std::clamp (b, 0.0, 0.999) * 256.0);
    uint8_t A = static_cast<uint8_t> (std::clamp (a, 0.0, 0.999) * 256.0);

    return (A << 24) | (B << 16) | (G << 8) | R;
}

void AppLayer::RayTracer (Image &target) 
{ 
    // If camera is not available, fall back to test pattern
    if (!m_Camera)
    {
        GenerateTestPattern (target);
        return;
    }

    const uint32_t width = target.GetWidth ();
    const uint32_t height = target.GetHeight ();

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            Color color (0, 0, 0);
            for (int sample = 0; sample < m_Camera->GetSamplesPerPixel(); sample++)
            {
                // get ray for this pixel from camera
                Ray r = m_Camera->GetRay (x, y);
                color += m_Camera->RayColor (r, m_Camera->GetMaxDepth (), m_World);                
            }
            
            color *= m_Camera->GetPixelSamplesScale ();
            auto packed = PackColor (color.GetX (), color.GetY (), color.GetZ (), 1.0);
            target.SetPixel (x, y, packed);
        }
    }
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
    auto &renderer = Application::Get().GetRenderer();

    // If front image has no GPU resource yet, generate & upload synchronously
    // on the calling thread (which is the UI / render thread). This ensures
    // the first button press immediately shows an image without doing GPU
    // work on the worker thread.
    if (m_FrontImage && !m_FrontImage->GetSRV())
    {
        Logger::Log(Logger::Level::INFO, "First render (sync) - generating into front buffer");
        auto start = std::chrono::high_resolution_clock::now();

        RayTracer (*m_FrontImage);
        m_FrontImage->UpdateGPUTexture(renderer.GetDevice(), renderer.GetDeviceContext());
        m_BackReady.store(false, std::memory_order_release);

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        m_LastRenderTimeMs.store(ms, std::memory_order_release);
        return;
    }

    // Normal: schedule heavy work on background worker into back buffer
    m_CommandQueue.Push([this]()
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (m_BackImage)
        {
            Logger::Log(Logger::Level::INFO, "Generating into back buffer");
            RayTracer (*m_BackImage);

            // publish completed back buffer to render thread
            m_BackReady.store(true, std::memory_order_release);
        }

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        m_LastRenderTimeMs.store(ms, std::memory_order_release);
    });
}

