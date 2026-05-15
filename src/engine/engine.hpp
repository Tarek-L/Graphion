#pragma once

// =============================================================================
//  ENGINE CORE
// =============================================================================


// -----------------------------------------------------------------------------
//  TRANSFORM
//  TRS model matrix with dirty-flag caching.
//  Public members — call setDirty() manually after changing any of them.
//
//    transform.position   = glm::vec3(x, y, z)
//    transform.rotationX  = float          // degrees
//    transform.rotationY  = float          // degrees
//    transform.rotationZ  = float          // degrees
//    transform.scale      = glm::vec3(sx, sy, sz)
//    transform.setDirty()                  // mark matrix as stale
//
//    transform.getModelMatrix()  →  const glm::mat4&   // lazily recomputed
// -----------------------------------------------------------------------------
#include "include/transform.hpp"


// -----------------------------------------------------------------------------
//  MESH
//  Thin VAO/VBO/EBO wrapper. Created by Registry — don't construct manually.
//  Vertex layout:  location 0 = position (vec3)
//                  location 1 = color    (vec4)
//                  location 2 = texcoord (vec2)  [unused for now]
//
//    mesh.bind()                      // glBindVertexArray
//    mesh.unbind()                    // glBindVertexArray(0)
//    mesh.getVertexCount()  →  int    // index count passed to glDrawElements
// -----------------------------------------------------------------------------
#include "include/mesh.hpp"


// -----------------------------------------------------------------------------
//  SHADER
//  Loads vertex + fragment GLSL from file. Caches uniform locations.
//
//    Shader sh("path/vert.glsl", "path/frag.glsl");
//    sh.use()                                    // glUseProgram
//
//  Uniform setters (all look up by name, cached after first call):
//    sh.uniform("name", bool)
//    sh.uniform("name", int)
//    sh.uniform("name", float)
//    sh.uniform("name", glm::vec2/vec3/vec4)
//    sh.uniform("name", glm::mat4)
// -----------------------------------------------------------------------------
#include "include/shader.hpp"


// -----------------------------------------------------------------------------
//  ENTITY
//  Base drawable object. Holds a Mesh*, Shader*, Transform, and colour.
//  Non-owning — mesh and shader are managed by Registry.
//
//    Entity e(mesh, shader, glm::vec4(r, g, b, a));
//
//  Fields (public):
//    e.transform                      // Transform — position, rotation, scale
//    e.color                          // glm::vec4 — multiplied with vColor in shader
//
//  Methods:
//    e.getMesh()        →  Mesh*
//    e.getShader()      →  Shader*
//    e.getPosition()    →  glm::vec3
//    e.move(delta)                    // translate position by glm::vec3 delta
// -----------------------------------------------------------------------------
#include "include/entity.hpp"


// =============================================================================
//  CAMERA  &  RENDERER
// =============================================================================


// -----------------------------------------------------------------------------
//  CAMERA  [static — no instances]
//  2D orthographic camera. One camera exists for the app lifetime.
//
//  Startup (call once after GL context is ready):
//    Camera::init(width, height)
//
//  On resize (call from your framebuffer callback):
//    Camera::updateProjection(newWidth, newHeight)
//
//  Per-frame movement:
//    Camera::setPosition(x, y)
//    Camera::move(dx, dy)             // additive translate
//    Camera::getPosition()  →  glm::vec2
//
//  Zoom  (clamped to [0.01, 100]):
//    Camera::setZoom(float)           // absolute
//    Camera::zoomBy(factor)           // multiplicative: zoom *= factor
//    Camera::getZoom()  →  float
//
//  Matrices (lazily computed, returned by const-ref — no copy):
//    Camera::getViewMatrix()        →  const glm::mat4&
//    Camera::getProjectionMatrix()  →  const glm::mat4&
// -----------------------------------------------------------------------------
#include "include/camera.hpp"


// -----------------------------------------------------------------------------
//  RENDERER  [static — no instances]
//  Submits Entity objects to the GPU using the global Camera matrices.
//  Clearing the framebuffer is handled by Window::beginFrame(), not Renderer.
//
//  Startup (call once after GL context is ready):
//    Renderer::init()                 // enables blending + depth test
//
//  Per draw call:
//    Renderer::draw(entity)           // binds shader, sets uModel/uView/
//                                     // uProjection/uColor, draws mesh
// -----------------------------------------------------------------------------
#include "include/renderer.hpp"


// =============================================================================
//  ASSET MANAGEMENT
// =============================================================================


// -----------------------------------------------------------------------------
//  REGISTRY
//  Lazy asset factory. Creates and caches Mesh and Shader objects by name.
//  All returned pointers are owned by the Registry — do not delete them.
//
//    Registry registry;
//    Mesh*   m = registry.getMesh("circle");     // or "square" "triangle"
//    Shader* s = registry.getShader("basic");
//
//  Built-in meshes:
//    "circle"         — ~270-triangle approximation
//    "square"         — unit quad centered at origin
//    "triangle"       — equilateral triangle
//    "errorTriangle"  — fallback shown when a mesh is missing
//
//  Built-in shaders:
//    "basic"          — uses uModel / uView / uProjection / uColor
// -----------------------------------------------------------------------------
#include "include/registry.hpp"


// =============================================================================
//  GRAPH  SYSTEM
// =============================================================================


// -----------------------------------------------------------------------------
//  VERTEX  :  Entity
//  A graph node rendered as a circle. Extends Entity with 2D position helpers.
//
//    Vertex* v = graph.addVertex({x, y}, color);   // prefer Graph::addVertex
//
//  Position (z is managed internally, always 0.0):
//    v->setPos(glm::vec2)
//    v->setPos(float x, float y)
//    v->getPos()  →  glm::vec2
//
//  Metadata (set by Graph):
//    v->id       int           auto-incremented
//    v->label    std::string   optional display name
// -----------------------------------------------------------------------------
#include "include/vertex.hpp"


// -----------------------------------------------------------------------------
//  EDGE  :  Entity
//  A connection between two Vertices rendered as a scaled rectangle.
//  Call refresh() whenever a connected vertex moves.
//
//    Edge* e = graph.addEdge(from, to, color);     // prefer Graph::addEdge
//
//    e->refresh()                      // recompute transform from vertex positions
//    e->getFrom()      →  Vertex*
//    e->getTo()        →  Vertex*
//    e->connects(v)    →  bool         // is v an endpoint?
//    e->connects(a, b) →  bool         // does this edge connect a and b?
//    e->thickness      float           // world-space height of the rectangle
// -----------------------------------------------------------------------------
#include "include/edge.hpp"


// -----------------------------------------------------------------------------
//  GRAPH
//  Owns and manages a set of Vertices and Edges. Handles drawing order
//  (edges behind, vertices on top) and edge-position synchronisation.
//
//    Graph graph(registry);                        // default radius + thickness
//    Graph graph(registry, 0.3f, 0.05f);           // custom sizes
//
//  Building:
//    Vertex* v = graph.addVertex({x, y}, color)
//    Edge*   e = graph.addEdge(from, to, color)
//    graph.removeVertex(v)             // also removes connected edges
//    graph.removeEdge(e)
//
//  Per-frame:
//    graph.refresh()                   // snap all edges to current vertex positions
//    graph.draw()                      // edges first, then vertices
//
//  Accessors:
//    graph.vertices()      →  const vector<unique_ptr<Vertex>>&
//    graph.edges()         →  const vector<unique_ptr<Edge>>&
//    graph.vertexCount()   →  int
//    graph.edgeCount()     →  int
// -----------------------------------------------------------------------------
#include "include/graph.hpp"


// =============================================================================
//  FUNCTION  PLOTTING
// =============================================================================


// -----------------------------------------------------------------------------
//  FUNCTION PLOT
//  Samples f(x) and renders it as a polyline of scaled rectangle segments.
//  Discontinuities (1/x, log(-1), sqrt(-1) …) produce silent gaps in the curve.
//
//    FunctionPlot p(registry, "sin(x)", -6.28f, 6.28f);
//    FunctionPlot p(registry, "x^2 - 3*x + 1");
//    FunctionPlot p(registry, "exp(-x^2)");           // Gaussian
//
//  Supported syntax:
//    Operators  :  +  -  *  /  ^  (unary minus)
//    Variable   :  x
//    Constants  :  pi   e
//    Functions  :  sin  cos  tan  asin  acos  atan
//                  exp  ln  log  log2  log10
//                  sqrt  abs  floor  ceil  round
//
//  Setters (each triggers a rebuild except setColor):
//    p.setExpression("cos(x)")
//    p.setRange(xMin, xMax)
//    p.setSamples(int)                 // default 300 — more = smoother
//    p.setColor(glm::vec4)             // O(n) recolor, no resample
//    p.setThickness(float)             // world-space line width
//
//  Per-frame:
//    p.draw()
//
//  Diagnostics:
//    p.isValid()        →  bool        // false if expression failed to parse
//    p.segmentCount()   →  int         // number of rendered line segments
//    p.getExpression()  →  const std::string&
// -----------------------------------------------------------------------------
#include "include/function.hpp"
