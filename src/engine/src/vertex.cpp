

//vertex.cpp

#include "vertex.hpp"

Vertex::Vertex(Mesh* mesh, Shader* shader, glm::vec2 position, glm::vec4 color)
    : Entity(mesh, shader, color)
{
    // Bake position into transform, keep z fixed to this layer
    transform.position = glm::vec3(position, Z_LAYER);
}

void Vertex::setPos(glm::vec2 pos)
{
    transform.position.x = pos.x;
    transform.position.y = pos.y;
    // z stays at Z_LAYER — don't touch it
    transform.setDirty();
}

void Vertex::setPos(float x, float y)
{
    setPos({ x, y });
}

glm::vec2 Vertex::getPos() const
{
    return glm::vec2(transform.position.x, transform.position.y);
}
