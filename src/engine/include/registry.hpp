
#pragma once 
#include <unordered_map>
#include <memory>
#include "mesh.hpp"
#include "shader.hpp"

class Registry {
private:
    std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Meshes;
    std::unordered_map<std::string, std::unique_ptr<Shader>> m_Shaders;
    Mesh* m_createCircle();
    Mesh* m_createSquare();
    Mesh* m_createTriangle();
    Mesh* m_createErrorTriangle();
public:
    Mesh* getMesh(const char* name);
    Shader* getShader(const char* name);
};
