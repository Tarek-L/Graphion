#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline const char* GLGetErrorString(GLenum error) {
    switch (error) {
        case GL_INVALID_ENUM:                  return "INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
        case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
        default:                               return "UNKNOWN_ERROR";
    }
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    bool hasError = false;
    while (GLenum error = glGetError()) {
        spdlog::error("[OpenGL Error] ({}) | Call: {} | {}:{}", 
                      GLGetErrorString(error), function, file, line);
        hasError = true;
    }
    return hasError;
}

#if defined(_MSC_VER)
    #define GL_DEBUG_BREAK() __debugbreak()
#else
    #define GL_DEBUG_BREAK() __builtin_trap()
#endif

#ifdef NDEBUG
    #define GL_CALL(x) x
#else
    #define GL_CALL(x) do { \
        GLClearError(); \
        x; \
        if (GLLogCall(#x, __FILE__, __LINE__)) { \
            GL_DEBUG_BREAK(); \
        } \
    } while (0)
#endif

#ifdef NDEBUG
    #define ASSERT(x) ((void)0)
#else
    #define ASSERT(x) do { \
        if (!(x)) { \
            spdlog::error("[Assertion Failed] {} | {}:{}", #x, __FILE__, __LINE__); \
            GL_DEBUG_BREAK(); \
        } \
    } while (0)
#endif

#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_INFO(...)  spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)  spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
