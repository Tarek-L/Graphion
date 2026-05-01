
#include "transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Transform::setDirty() {
    m_isDirty = true;
}

glm::mat4 Transform::getModelMatrix() const {
    if (!m_isDirty) {
        return m_matrix;
    }

    // 1. Start fresh (Identity)
    glm::mat4 model = glm::mat4(1.0f);

    // 2. Apply Absolute Transformations (Order matters: TRS)
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0, 0, 1));

    model = glm::scale(model, scale);

    // 3. Cache the result
    m_matrix = model;
    m_isDirty = false;

    return m_matrix;
}
