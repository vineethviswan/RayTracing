#pragma once


#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include "../core/source/CommandQueue.h"
#include "Image.h"
#include "Layer.h"
#include "../core/source/Constants.h"
#include "../core/source/Camera.h"

class AppLayer : public Layer
{
public:
    AppLayer (std::shared_ptr<Image> image);
    ~AppLayer ();

    virtual void OnUpdate (double ts) override;
    virtual void OnRender () override;
    virtual void OnAttach () override;

    // Generate a test pattern into the provided image (target may be front or back buffer)
    void GenerateTestPattern (Image &target);

    // Actual Ray Tracing render function
    void RayTracer (Image &target);

    static uint32_t PackColor (double r, double g, double b, double a = 1.0);

    // Enqueue a heavy render job (called from UI thread)
    void EnqueueRenderJob ();

    double GetLastRenderTimeMs() const { return m_LastRenderTimeMs.load(std::memory_order_acquire); }

private:
    // Double-buffering: front used for GPU upload/display, back used by worker
    std::shared_ptr<Image> m_FrontImage;
    std::shared_ptr<Image> m_BackImage;

    std::unique_ptr<Camera> m_Camera;

    std::atomic<bool> m_BackReady {false};
    std::mutex m_SwapMutex;

    CommandQueue m_CommandQueue;
    std::thread m_Worker;
    std::atomic<bool> m_WorkerRunning {false};
    std::atomic<double> m_LastRenderTimeMs {0.0};
};
