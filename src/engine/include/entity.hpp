
#pragma once 
#include "mesh.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Entity {
private:
    Mesh* m_Mesh;
    Shader* m_Shader;

public:
    Transform transform;
    glm::vec4 color;
    
    Entity(Mesh* mesh, Shader* shader, glm::vec4 color = glm::vec4(1.0f)) {
        m_Mesh = mesh;
        m_Shader = shader;
        this->color = color;
    }
    
    glm::vec3 getPosition();
    void move(glm::vec3 delta);

    Mesh* getMesh() const;
    Shader* getShader() const;
};


