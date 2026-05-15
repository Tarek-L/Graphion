
//edge.cpp
#include "edge.hpp"
#include "vertex.hpp"

#include <glm/glm.hpp>
#include <cmath>

Edge::Edge(Mesh* mesh, Shader* shader,
           Vertex* from, Vertex* to,
           glm::vec4 color, float thickness)
    : Entity(mesh, shader, color)
    , m_from(from)
    , m_to(to)
    , thickness(thickness)
{
    refresh();
}

void Edge::refresh()
{
    glm::vec2 a = m_from->getPos();
    glm::vec2 b = m_to->getPos();

    glm::vec2 delta  = b - a;
    float     length = glm::length(delta);

    // Midpoint — where the rectangle is centered
    glm::vec2 mid = (a + b) * 0.5f;

    // Angle from a to b (in degrees, around Z axis)
    float angleDeg = glm::degrees(std::atan2(delta.y, delta.x));

    transform.position  = glm::vec3(mid, Z_LAYER);
    transform.rotationZ = angleDeg;
    transform.scale     = glm::vec3(length, thickness, 1.0f);
    transform.setDirty();
}

bool Edge::connects(const Vertex* v) const
{
    return m_from == v || m_to == v;
}

bool Edge::connects(const Vertex* a, const Vertex* b) const
{
    return (m_from == a && m_to == b)
        || (m_from == b && m_to == a);
}
