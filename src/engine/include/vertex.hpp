
//vertex.hpp
#pragma once

#include "entity.hpp"
#include <glm/glm.hpp>
#include <string>

class Vertex : public Entity
{
public:
    Vertex(Mesh*     mesh,
           Shader*   shader,
           glm::vec2 position,
           glm::vec4 color = glm::vec4(0.3f, 0.7f, 1.0f, 1.0f));

    void      setPos(glm::vec2 pos);
    void      setPos(float x, float y);
    glm::vec2 getPos() const;
    int         id    = -1;
    std::string label = "";

    static constexpr float Z_LAYER = 0.0f;
};
