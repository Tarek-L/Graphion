//camera.hpp
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
   /** Must be called once after the OpenGL context is ready. */
    static void init(float width, float height);

    /** Call from your framebuffer-resize callback. */
    static void updateProjection(float width, float height);
    static void        setPosition(float x, float y);
    static void        move(float dx, float dy);
    static glm::vec2   getPosition();

    static void  setZoom(float zoom);
    static void  zoomBy(float factor);   // multiplicative: zoom *= factor
    static float getZoom();

    static const glm::mat4& getViewMatrix();
    static const glm::mat4& getProjectionMatrix();

    Camera()             = delete;
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

private:
    static void recalculate();

    static constexpr float MIN_ZOOM = 0.01f;
    static constexpr float MAX_ZOOM = 100.0f;

    static inline glm::vec2  s_position   = { 0.0f,          0.0f         };
    static inline float      s_zoom       = 1.0f;
    static inline float      s_width      = 0.0f;
    static inline float      s_height     = 0.0f;
    static inline bool       s_dirty      = true;
    static inline glm::mat4  s_view       = glm::mat4(1.0f);
    static inline glm::mat4  s_projection = glm::mat4(1.0f);
};
