#pragma once

#include <log/logging.hpp>
#include <GLFW/glfw3.h>
#include <vendor/imgui/backends/imgui_impl_glfw.h>
#include <vendor/imgui/backends/imgui_impl_opengl3.h>

namespace Graphion::GUI {

class GUI {
public:
    GUI() = default;
    ~GUI() = default;

    void init(GLFWwindow* window) {
        IMGUI_CHECKVERSION();

        m_context = ImGui::CreateContext();
        ASSERT(m_context, "Failed to create ImGui context");

        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        // Config
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Style
        ImGui::StyleColorsDark();

        setupStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::SetCurrentContext(m_context);
        LOG_INFO("ImGui initialized");
    }

    void shutdown() {
        if (m_context) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(m_context);
            m_context = nullptr;

            LOG_INFO("ImGui shutdown");
        }
    }

    void beginFrame() {
        ASSERT(m_context, "ImGui not initialized");
        ImGui::SetCurrentContext(m_context);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();

        ImGui::NewFrame();
    }

    void endFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void demoWindow(bool* open = nullptr) {
        ImGui::ShowDemoWindow(open);
    }

    bool beginWindow(const char* name,
                     bool* open = nullptr,
                     ImGuiWindowFlags flags = 0) {
        return ImGui::Begin(name, open, flags);
    }

    void endWindow() {
        ImGui::End();
    }

    void text(const char* text) {
        ImGui::TextUnformatted(text);
    }

    template<typename... Args>
    void text(const char* fmt, Args&&... args) {
        ImGui::Text(fmt, std::forward<Args>(args)...);
    }

    bool button(const char* label,
                const ImVec2& size = ImVec2(0, 0)) {
        return ImGui::Button(label, size);
    }

    bool checkbox(const char* label, bool* value) {
        return ImGui::Checkbox(label, value);
    }

    bool sliderFloat(const char* label,
                     float* value,
                     float min,
                     float max) {
        return ImGui::SliderFloat(label, value, min, max);
    }

    bool sliderInt(const char* label,
                   int* value,
                   int min,
                   int max) {
        return ImGui::SliderInt(label, value, min, max);
    }

    bool inputFloat(const char* label, float* value) {
        return ImGui::InputFloat(label, value);
    }

    bool inputInt(const char* label, int* value) {
        return ImGui::InputInt(label, value);
    }

    bool inputText(const char* label,
                   char* buffer,
                   size_t bufferSize) {
        return ImGui::InputText(label, buffer, bufferSize);
    }

    void separator() {
        ImGui::Separator();
    }

    void sameLine() {
        ImGui::SameLine();
    }

    void spacing() {
        ImGui::Spacing();
    }

    void setDarkTheme() {
        ImGui::StyleColorsDark();
    }

    void setLightTheme() {
        ImGui::StyleColorsLight();
    }

    void setClassicTheme() {
        ImGui::StyleColorsClassic();
    }

private:
    void setupStyle() {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding = 6.0f;
        style.FrameRounding  = 4.0f;
        style.PopupRounding  = 4.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;

        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize  = 0.0f;

        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding  = ImVec2(8, 4);
        style.ItemSpacing   = ImVec2(8, 6);
    }

private:
    ImGuiContext* m_context = nullptr;
};

} // namespace Graphion::GUI
