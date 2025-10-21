#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include "Logger.h"

// Simple thread-safe command queue for worker threads
class CommandQueue
{
public:
    void Push (std::function<void ()> cmd)
    {
        {
            std::lock_guard<std::mutex> lk (m_Mutex);
            m_Queue.push (std::move (cmd));
        }
        Logger::Log (Logger::Level::DEBUG, "CommandQueue: job pushed");
        m_Cv.notify_one ();
    }

    // Blocks until a command is available or shutdown is signaled
    std::function<void ()> PopBlocking ()
    {
        std::unique_lock<std::mutex> lk (m_Mutex);
        m_Cv.wait (lk, [this] () { return !m_Queue.empty () || m_Shutdown; });
        if (m_Queue.empty ())
        {
            Logger::Log (Logger::Level::DEBUG, "CommandQueue: shutdown signaled, returning null job");
            return nullptr;
        }
        auto cmd = std::move (m_Queue.front ());
        m_Queue.pop ();
        Logger::Log (Logger::Level::DEBUG, "CommandQueue: job popped");
        return cmd;
    }

    void Shutdown ()
    {
        {
            std::lock_guard<std::mutex> lk (m_Mutex);
            m_Shutdown = true;
        }
        Logger::Log (Logger::Level::INFO, "CommandQueue: shutdown requested");
        m_Cv.notify_all ();
    }

private:
    std::mutex m_Mutex;
    std::condition_variable m_Cv;
    std::queue<std::function<void ()>> m_Queue;
    bool m_Shutdown = false;
};
