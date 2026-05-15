
//graph.cpp
#include "graph.hpp"
#include "renderer.hpp"

#include <logging.hpp>
#include <algorithm>

Graph::Graph(Registry& registry, float vertexRadius, float edgeThickness)
    : m_vertexRadius(vertexRadius)
    , m_edgeThickness(edgeThickness)
{
    m_circleMesh = registry.getMesh("circle");
    m_squareMesh = registry.getMesh("square");
    m_shader     = registry.getShader("basic");

    ASSERT(m_circleMesh, "Graph: Registry did not return a 'circle' mesh");
    ASSERT(m_squareMesh, "Graph: Registry did not return a 'square' mesh");
    ASSERT(m_shader,     "Graph: Registry did not return a 'basic' shader");
}

Vertex* Graph::addVertex(glm::vec2 position, glm::vec4 color)
{
    auto v = std::make_unique<Vertex>(m_circleMesh, m_shader, position, color);

    v->id = m_nextId++;
    v->transform.scale = glm::vec3(m_vertexRadius);

    Vertex* ptr = v.get();
    m_vertices.push_back(std::move(v));

    LOG_INFO("Graph: added vertex {} at ({:.2f}, {:.2f})",
             ptr->id, position.x, position.y);

    return ptr;
}

Edge* Graph::addEdge(Vertex* from, Vertex* to, glm::vec4 color)
{
    ASSERT(from, "Graph::addEdge — 'from' is null");
    ASSERT(to,   "Graph::addEdge — 'to'   is null");
    ASSERT(from != to, "Graph::addEdge — self-loop not supported");

    auto e   = std::make_unique<Edge>(m_squareMesh, m_shader,
                                      from, to, color, m_edgeThickness);
    Edge* ptr = e.get();
    m_edges.push_back(std::move(e));

    LOG_INFO("Graph: added edge {} → {}", from->id, to->id);

    return ptr;
}

void Graph::removeEdge(Edge* e)
{
    m_edges.erase(
        std::remove_if(m_edges.begin(), m_edges.end(),
            [e](const std::unique_ptr<Edge>& ptr){ return ptr.get() == e; }),
        m_edges.end()
    );
}

void Graph::removeVertex(Vertex* v)
{
    ASSERT(v, "Graph::removeVertex — vertex is null");

    // Cascade: remove every edge that touches this vertex first
    m_edges.erase(
        std::remove_if(m_edges.begin(), m_edges.end(),
            [v](const std::unique_ptr<Edge>& e){ return e->connects(v); }),
        m_edges.end()
    );

    m_vertices.erase(
        std::remove_if(m_vertices.begin(), m_vertices.end(),
            [v](const std::unique_ptr<Vertex>& ptr){ return ptr.get() == v; }),
        m_vertices.end()
    );
}
void Graph::refresh()
{
    for (auto& e : m_edges)
        e->refresh();
}

void Graph::draw() const
{
    // Edges first — they live at a lower z so depth test puts them behind
    for (const auto& e : m_edges)
        Renderer::draw(*e);

    // Vertices on top
    for (const auto& v : m_vertices)
        Renderer::draw(*v);
}
