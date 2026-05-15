
//edge.hpp
#pragma once

#include "entity.hpp"
#include <glm/glm.hpp>

// Forward declare — Edge only needs Vertex's interface, not the full type
class Vertex;
class Edge : public Entity
{
public:
    Edge(Mesh*     mesh,
         Shader*   shader,
         Vertex*   from,
         Vertex*   to,
         glm::vec4 color     = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f),
         float     thickness = 0.05f);

    void refresh();

    Vertex* getFrom() const { return m_from; }
    Vertex* getTo()   const { return m_to;   }

    bool connects(const Vertex* v)                    const;

    bool connects(const Vertex* a, const Vertex* b)   const;

    float thickness;

    static constexpr float Z_LAYER = -0.1f;

private:
    Vertex* m_from;
    Vertex* m_to;
};
