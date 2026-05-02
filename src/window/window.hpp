#pragma once
#include <GLFW/glfw3.h>
#include <init/color.hpp>

namespace Graphion::Window {

class Window {

  private:
    int m_CanvaWidth, m_CanvaHeight;
    float m_aspectRatio;
    GLFWwindow *m_handle = nullptr;

    void onResize(int widthOfFramebuffer, int heightOfFramebuffer);
    static void framebufferSizeCallback(GLFWwindow *window, int w, int h);

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

    void beginFrame();
    void endFrame();

    GLFWwindow *getHandle() const { return m_handle; }
    bool isValid() const noexcept { return glfwWindowShouldClose(m_handle); }

};

} // namespace Graphion::window
