/**
 * @file clock.hpp
 * @brief High-resolution game timing and frame control utilities for GLFW/OpenGL applications.
 *
 * This header provides a lightweight Clock class designed to mimic engine-level
 * timing systems such as Pygame's `Clock`. It abstracts time measurement and frame
 * pacing using GLFW ( and or std::chrono fallback logic and for duration types),
 * and is intended for use in real-time rendering loops.
 *
 * Core features:
 *  - High-precision delta time calculation (time between frames)
 *  - FPS (frames per second) measurement
 *  - Frame-rate limiting (similar to pygame.time.Clock.tick)
 *  - Global time scaling (slow motion / fast forward effects)
 *  - Simple time utilities (current time, sleep helper)
 *  - GLFW-based timing abstraction for OpenGL applications
 *
 * Typical usage:
 *
 *  Clock clock;
 *
 *  while (!window.isValid())
 *  {
 *      float dt = clock.tick();   // compute delta time + FPS
 *
 *      glfwSwapBuffers(window);
 *      glfwPollEvents();
 *
 *      clock.limitFPS(60.0f);     // optional FPS cap
 *  }
 *
 * Delta Time Usage:
 *  Always multiply movement, physics, and animation values by dt:
 *
 *      position += velocity * dt;
 *
 * This ensures consistent behavior across different hardware and frame rates.
 *
 * Time Scaling:
 *  The clock supports global time scaling:
 *
 *      clock.setTimeScale(0.5f); // slow motion
 *      clock.setTimeScale(2.0f); // fast forward
 *
 *  All delta time outputs are affected by this scale factor.
 *
 * Frame Limiting:
 *  limitFPS() prevents the main loop from running faster than a target FPS
 *  by sleeping the thread for the remaining frame time.
 *
 * Static Utilities:
 *  - Clock::now()     → returns current time in seconds (GLFW-based)
 *  - Clock::nowMs()   → current time in milliseconds
 *  - Clock::sleep(x)  → thread sleep helper in seconds
 *
 * Design Notes:
 *  This class is intended for simple to medium-complexity engines.
 *  It does not implement fixed timestep simulation or interpolation;
 *  those can be built on top of this system if needed.
 *
 * Dependencies:
 *  - GLFW (for glfwGetTime)
 *  - <chrono> (fallback timing and sleep utilities)
 *  - <thread>
 */
#pragma once

#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

namespace Graphion::Clock {
class Clock
{
public:
    Clock()
    {
        reset();
    }

    float tick()
    {
        double now = getTime();

        deltaTime = static_cast<float>(now - lastTime);
        lastTime = now;

        fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;

        return deltaTime * timeScale;
    }

    void limitFPS(float targetFPS)
    {
        if (m_vsyncEnable) return;
        double frameTime = getTime() - lastFrameStart;
        double targetFrameTime = 1.0 / targetFPS;

        if (frameTime < targetFrameTime)
        {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(targetFrameTime - frameTime)
            );
        }

        lastFrameStart = getTime();
    }

    void vSyncON(bool ON){
        m_vsyncEnable = ON;
        if (m_vsyncEnable)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    float getDeltaTime() const { return deltaTime * timeScale; }
    float getRawDeltaTime() const { return deltaTime; }
    float getFPS() const { return fps; }

    void setTimeScale(float scale)
    {
        timeScale = std::max(0.0f, scale);
    }

    float getTimeScale() const
    {
        return timeScale;
    }

    void reset()
    {
        lastTime = getTime();
        lastFrameStart = lastTime;
        deltaTime = 0.0f;
        fps = 0.0f;
    }

    static double now()
    {
        return getTime();
    }

    static double nowMs()
    {
        return getTime() * 1000.0;
    }

    static void sleep(float seconds)
    {
        std::this_thread::sleep_for(
            std::chrono::duration<float>(seconds)
        );
    }

private:
    static double getTime()
    {
        return glfwGetTime();
    }

private:
    bool m_vsyncEnable = false;
    double lastTime = 0.0;
    double lastFrameStart = 0.0;

    float deltaTime = 0.0f;
    float fps = 0.0f;

    float timeScale = 1.0f;
};
}
