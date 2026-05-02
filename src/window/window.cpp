
#include <log/logging.hpp>
#include <window/window.hpp>

namespace Graphion::Window {

static int SCREEN_WIDTH = 0;
static int SCREEN_HEIGHT = 0;

void initMonitor() {
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primary);
    SCREEN_WIDTH = mode->width;
    SCREEN_HEIGHT = mode->height;
}

void Window::onResize(int widthOfFramebuffer, int heightOfFramebuffer) {
    float desiredAspectRatio = m_aspectRatio;

    int widthOfViewport, heightOfViewport;
    int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY;

    float requiredHeightOfViewport =
        widthOfFramebuffer * (1.0f / desiredAspectRatio);
    if (requiredHeightOfViewport > heightOfFramebuffer) {
        float requiredWidthOfViewport =
            heightOfFramebuffer * desiredAspectRatio;
        ASSERT(requiredWidthOfViewport <= widthOfFramebuffer,
               "Error: Couldn't find dimensions that preserve the aspect "
               "ratio\n");

        widthOfViewport = static_cast<int>(requiredWidthOfViewport);
        heightOfViewport = heightOfFramebuffer;

        float widthOfTheTwoVerticalBars =
            widthOfFramebuffer - widthOfViewport;

        lowerLeftCornerOfViewportX =
            static_cast<int>(widthOfTheTwoVerticalBars / 2.0f);

        lowerLeftCornerOfViewportY = 0;
    } else {
        widthOfViewport = widthOfFramebuffer;
        heightOfViewport = static_cast<int>(requiredHeightOfViewport);

        float heightOfTheTwoHorizontalBars =
            heightOfFramebuffer - heightOfViewport;

        lowerLeftCornerOfViewportX = 0;
        lowerLeftCornerOfViewportY =
            static_cast<int>(heightOfTheTwoHorizontalBars / 2.0f);
    }

    GL_CALL(glViewport(lowerLeftCornerOfViewportX,
                       lowerLeftCornerOfViewportY, widthOfViewport,
                       heightOfViewport));
    GL_CALL(glScissor(lowerLeftCornerOfViewportX,
                      lowerLeftCornerOfViewportY, widthOfViewport,
                      heightOfViewport));
}

void Window::framebufferSizeCallback(GLFWwindow *window, int w, int h) {
    Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    self->onResize(w, h);
}

Window::Window(const char *title, int width, int height) : m_handle(nullptr) {

    ASSERT(glfwInit(), "Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    initMonitor();

    ASSERT(width <= SCREEN_WIDTH && height <= SCREEN_HEIGHT,
           "invalid window size");
    ASSERT(width > 0 && height > 0, "invalid window size");

    m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    ASSERT(m_handle, "Failed to create GLFW window");

    glfwMakeContextCurrent(m_handle);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
           "Failed to initialize GLAD");

    LOG_INFO("initialized (GLFW + GLAD)");

    m_CanvaWidth = width;
    m_CanvaHeight = height;

    m_aspectRatio = static_cast<float>(m_CanvaWidth) / m_CanvaHeight;

    ASSERT(m_handle, "window is null");
    glfwSetWindowUserPointer(m_handle, this);
    ASSERT(m_handle, "window is null");
    glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
    eventSetupCallbacks();
}

Window::Window(Window &&other) noexcept
    : m_CanvaWidth(other.m_CanvaWidth),
      m_CanvaHeight(other.m_CanvaHeight),
      m_aspectRatio(other.m_aspectRatio),
      m_handle(other.m_handle),
      foreground(other.foreground),
      background(other.background)
{
    other.m_handle = nullptr;

    if (m_handle)
        glfwSetWindowUserPointer(m_handle, this);
}

Window &Window::operator=(Window &&other) noexcept {
    if (this != &other) {
        if (m_handle)
            glfwDestroyWindow(m_handle);

        m_handle = other.m_handle;
        other.m_handle = nullptr;
        m_CanvaWidth = other.m_CanvaWidth;
        m_CanvaHeight = other.m_CanvaHeight;
        m_aspectRatio = other.m_aspectRatio;
        foreground = other.foreground;
        background = other.background;

        if (m_handle)
            glfwSetWindowUserPointer(m_handle, this);
    }
    return *this;
}


Window::~Window() {
    if (m_handle) {
        glfwDestroyWindow(m_handle);
        m_handle = nullptr;
    }
    LOG_INFO("window Destroyed");
}

int Window::getCanvaWidth() const { return m_CanvaWidth; }

int Window::getCanvaHeight() const { return m_CanvaHeight; }

void Window::setCanvaSize(int width, int height) {
    ASSERT(width <= SCREEN_WIDTH && height <= SCREEN_HEIGHT,
           "invalid window size");
    ASSERT(width > 0 && height > 0, "invalid window size");
    m_CanvaHeight = height;
    m_CanvaWidth = width;
    m_aspectRatio = static_cast<float>(m_CanvaWidth) / m_CanvaHeight;
    onResize(m_CanvaWidth, m_CanvaHeight);
}

int Window::getWidth() const {
    int w, h;
    ASSERT(m_handle, "window is null");
    glfwGetWindowSize(m_handle, &w, &h);
    return w;
}

int Window::getHeight() const {
    int w, h;
    ASSERT(m_handle, "window is null");
    glfwGetWindowSize(m_handle, &w, &h);
    return h;
}

int Window::getXPos() const {
    int x, y;
    ASSERT(m_handle, "window is null");
    glfwGetWindowPos(m_handle, &x, &y);
    return x;
}

int Window::getYPos() const {
    int x, y;
    ASSERT(m_handle, "window is null");
    glfwGetWindowPos(m_handle, &x, &y);
    return y;
}

void Window::setSize(int width, int height) {
    ASSERT(width > 0 && height > 0, "invalid position");
    ASSERT(width <= SCREEN_WIDTH && height <= SCREEN_HEIGHT, "invalid window size");
    ASSERT(m_handle, "window is null");
    glfwSetWindowSize(m_handle, width, height);
}

void Window::setPos(int x, int y) {
    ASSERT(m_handle, "window is null");
    glfwSetWindowPos(m_handle, x, y); 
}

void Window::eventSetupCallbacks()
{
    glfwSetKeyCallback(m_handle,
    [](GLFWwindow* w, int key, int, int action, int)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));

        Event e{};

        if (action == GLFW_PRESS)
        {
            e.type = EventType::KeyPressed;
            e.key.key = key;
            self->m_eventQueue.push(e);
            Input::setKeyDown(key);
        }
        else if (action == GLFW_RELEASE)
        {
            e.type = EventType::KeyReleased;
            e.key.key = key;
            self->m_eventQueue.push(e);
            Input::setKeyUp(key);
        }
    });

    glfwSetCursorPosCallback(m_handle,
    [](GLFWwindow* w, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));

        Event e{};
        e.type = EventType::MouseMoved;
        e.mouseMove.x = (float)x;
        e.mouseMove.y = (float)y;

        self->m_eventQueue.push(e);
    });

    glfwSetMouseButtonCallback(m_handle,
    [](GLFWwindow* w, int button, int action, int)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));

        Event e{};
        e.type = (action == GLFW_PRESS)
            ? EventType::MouseButtonPressed
            : EventType::MouseButtonReleased;

        e.mouseButton.button = button;

        self->m_eventQueue.push(e);
    });

    glfwSetWindowCloseCallback(m_handle,
    [](GLFWwindow* w)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));

        Event e{};
        e.type = EventType::WindowClose;

        self->m_eventQueue.push(e);
    });
}

void Window::poll()
{
    glfwPollEvents();
}

const std::vector<Event>& Window::events() const
{
    return m_eventQueue.events();
}

void Window::clearEvents()
{
    m_eventQueue.clear();
}

void Window::beginFrame() {

    poll();

    GL_CALL(glClearColor(background.r, background.g, background.b,
                         background.a));

    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    GL_CALL(glClearColor(foreground.r, foreground.g, foreground.b,
                         foreground.a));

    GL_CALL(glEnable(GL_SCISSOR_TEST));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    GL_CALL(glDisable(GL_SCISSOR_TEST));

}
void Window::endFrame() {
    glfwSwapBuffers(m_handle);
    Input::clearKeyState();
    clearEvents();
}

} 
