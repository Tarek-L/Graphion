#pragma once
#include <GLFW/glfw3.h>
#include <init/color.hpp>
#include <input/input.hpp>

namespace Graphion::Window {

class Window {

  private:
    EventQueue m_eventQueue;
    int m_CanvaWidth, m_CanvaHeight;
    float m_aspectRatio;
    GLFWwindow *m_handle = nullptr;

    void onResize(int widthOfFramebuffer, int heightOfFramebuffer);
    static void framebufferSizeCallback(GLFWwindow *window, int w, int h);
    void eventSetupCallbacks();
    void poll();
    void clearEvents();

  public:

    Color foreground, background;

    Window(const char *title, int width, int height);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&other) noexcept;
    Window &operator=(Window &&other) noexcept;

    int getCanvaWidth() const;
    int getCanvaHeight() const;
    void setCanvaSize(int width, int height);

    int getWidth() const;
    int getHeight() const;
    int getXPos() const;
    int getYPos() const;

    void setSize(int width, int height);
    void setPos(int x, int y);

    const std::vector<Event>& events() const;

    void beginFrame();
    void endFrame();

    GLFWwindow *getHandle() const { return m_handle; }
    bool isValid() const noexcept { return glfwWindowShouldClose(m_handle); }
    bool isValid(bool state) const noexcept { 
        glfwSetWindowShouldClose(m_handle, !state);
        return glfwWindowShouldClose(m_handle);
    }

};

} // namespace Graphion::window
