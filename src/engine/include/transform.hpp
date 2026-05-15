
//transform.hpp
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
private:
    mutable bool m_isDirty = true;
    mutable glm::mat4 m_matrix = glm::mat4(1.0f);

public:
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    void setDirty(); 
    glm::mat4 getModelMatrix() const;
};
