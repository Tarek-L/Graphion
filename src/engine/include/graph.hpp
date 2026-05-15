
//graph.hpp
#pragma once

#include "vertex.hpp"
#include "edge.hpp"
#include "registry.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>


class Graph
{
public:
    
    explicit Graph(Registry& registry,
                   float     vertexRadius   = 0.3f,
                   float     edgeThickness  = 0.05f);

    Vertex* addVertex(glm::vec2 position,
                      glm::vec4 color = glm::vec4(0.3f, 0.7f, 1.0f, 1.0f));

    Edge* addEdge(Vertex*   from,
                  Vertex*   to,
                  glm::vec4 color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

    void removeEdge(Edge* e);

    void removeVertex(Vertex* v);

    void refresh();

    void draw() const;

    const std::vector<std::unique_ptr<Vertex>>& vertices() const { return m_vertices; }
    const std::vector<std::unique_ptr<Edge>>&   edges()    const { return m_edges;    }

    int vertexCount() const { return static_cast<int>(m_vertices.size()); }
    int edgeCount()   const { return static_cast<int>(m_edges.size());    }

private:
    std::vector<std::unique_ptr<Vertex>> m_vertices;
    std::vector<std::unique_ptr<Edge>>   m_edges;

    Mesh*   m_circleMesh;    // used for vertices
    Mesh*   m_squareMesh;    // used for edges
    Shader* m_shader;

    float m_vertexRadius;
    float m_edgeThickness;

    int m_nextId = 0;        // auto-incremented vertex id
};
