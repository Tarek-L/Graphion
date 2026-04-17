/**
 * @file logging.hpp
 * @brief Lightweight debugging and logging utilities.
 *
 * This header provides helper functions and macros for:
 *  - Clearing and logging OpenGL errors
 *  - Wrapping OpenGL calls with automatic error checking (GL_CALL)
 *  - Assertion handling in debug builds (ASSERT)
 *  - Debug break abstraction across compilers
 *  - Logging shortcuts using spdlog
 *
 * Usage:
 *  Wrap OpenGL calls with GL_CALL(...) in debug builds to automatically
 *  detect and report errors:
 *
 *      GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
 *
 *  Use ASSERT(expr) for debug-only checks:
 *
 *      ASSERT(ptr != nullptr);
 *
 * Behavior:
 *  - In Debug:
 *      * GL_CALL clears previous errors, executes the call, logs new errors,
 *        and triggers a breakpoint if any occur.
 *      * ASSERT logs failure and triggers a breakpoint.
 *  - In Release (NDEBUG defined):
 *      * GL_CALL executes normally (no overhead).
 *      * ASSERT is disabled.
 *
 * Dependencies:
 *  - glad (OpenGL loader)
 *  - spdlog (logging)
 *
 * Notes:
 *  - Uses __builtin_trap() (GCC/Clang) or __debugbreak() (MSVC) for breakpoints.
 *  - Provides manual definitions for GL_STACK_OVERFLOW and GL_STACK_UNDERFLOW
 *    if not available in headers.
 */

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
    #define DEBUG_BREAK() __debugbreak()
#else
    #define DEBUG_BREAK() __builtin_trap()
#endif

#ifdef NDEBUG
    #define GL_CALL(x) x
#else
    #define GL_CALL(x) do { \
        GLClearError(); \
        x; \
        if (GLLogCall(#x, __FILE__, __LINE__)) { \
            DEBUG_BREAK(); \
        } \
    } while (0)
#endif

#ifdef NDEBUG
    #define ASSERT(expr) ((void)0)
#else
    #define ASSERT(expr) do { \
        if (!(expr)) { \
            spdlog::error("[Assertion Failed] {} | {}:{}", #expr, __FILE__, __LINE__); \
            DEBUG_BREAK(); \
        } \
    } while (0)
#endif

#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_INFO(...)  spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)  spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
