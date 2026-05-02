
#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    unsigned int m_ID;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    int m_getUniformLocation(const char* name);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    unsigned int getID();

    void use();

    void uniform(const char* name, bool value);
    void uniform(const char* name, int value);
    void uniform(const char* name, float value);
    void uniform(const char* name, int v1, int v2);
    void uniform(const char* name, int v1, int v2, int v3);
    void uniform(const char* name, int v1, int v2, int v3, int v4);
    void uniform(const char* name, float v1, float v2);
    void uniform(const char* name, float v1, float v2, float v3);
    void uniform(const char* name, float v1, float v2, float v3, float v4);
    void uniform(const char* name, glm::mat4 transform);
    void uniform(const char* name, glm::vec4 transform);
};

#endif
