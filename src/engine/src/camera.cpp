
//camera.hpp
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
void Camera::init(float width, float height)
{
    s_width  = width;
    s_height = height;
    s_dirty  = true;
}

void Camera::updateProjection(float width, float height)
{
    s_width  = width;
    s_height = height;
    s_dirty  = true;
}

void Camera::setPosition(float x, float y)
{
    s_position = { x, y };
    s_dirty    = true;
}

void Camera::move(float dx, float dy)
{
    s_position.x += dx;
    s_position.y += dy;
    s_dirty = true;
}

glm::vec2 Camera::getPosition()
{
    return s_position;
}

void Camera::setZoom(float zoom)
{
    s_zoom  = std::clamp(zoom, MIN_ZOOM, MAX_ZOOM);
    s_dirty = true;
}

void Camera::zoomBy(float factor)
{
    setZoom(s_zoom * factor);
}

float Camera::getZoom()
{
    return s_zoom;
}

const glm::mat4& Camera::getViewMatrix()
{
    if (s_dirty) recalculate();
    return s_view;
}

const glm::mat4& Camera::getProjectionMatrix()
{
    if (s_dirty) recalculate();
    return s_projection;
}

void Camera::recalculate()
{
    // --- View matrix ---
    // Translate the world opposite to the camera's position so that moving the
    // camera right shifts the world left, etc.
    s_view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-s_position, 0.0f)
    );

    // --- Projection matrix ---
    // Orthographic bounds centered on the origin.
    // Dividing by zoom shrinks the visible half-extents → zooms in.
    float halfW = (s_width  * 0.5f) / s_zoom;
    float halfH = (s_height * 0.5f) / s_zoom;

    s_projection = glm::ortho(
        -halfW,  halfW,   // left,  right
        -halfH,  halfH,   // bottom, top
        -1.0f,   1.0f     // near,  far  (sufficient for 2D)
    );

    s_dirty = false;
}
