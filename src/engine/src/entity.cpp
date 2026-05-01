

#include "entity.hpp"

Mesh* Entity::getMesh() const { return m_Mesh; }
Shader* Entity::getShader() const { return m_Shader; }

void Entity::move(glm::vec3 delta) {

    transform.position += delta;
    transform.setDirty();
}

glm::vec3 Entity::getPosition() {
    return transform.position;
}
