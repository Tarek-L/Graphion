#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.hpp"
#include "logging.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    GL_CALL(vertex = glCreateShader(GL_VERTEX_SHADER));
    GL_CALL(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GL_CALL(glCompileShader(vertex));
    // print compile errors if any
    GL_CALL(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if(!success)
    {
        GL_CALL(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    GL_CALL(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GL_CALL(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GL_CALL(glCompileShader(fragment));
    // print compile errors if any
    GL_CALL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if(!success)
    {
        GL_CALL(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    GL_CALL(m_ID = glCreateProgram());
    GL_CALL(glAttachShader(m_ID, vertex));
    GL_CALL(glAttachShader(m_ID, fragment));
    GL_CALL(glLinkProgram(m_ID));
    // print linking errors if any
    GL_CALL(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
    if(!success)
    {
        GL_CALL(glGetProgramInfoLog(m_ID, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    GL_CALL(glDeleteShader(vertex));
    GL_CALL(glDeleteShader(fragment));
}

Shader::~Shader() {
    GL_CALL(glDeleteProgram(m_ID));
}

int Shader::m_getUniformLocation(const char* name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    int loc;
    GL_CALL(loc = glGetUniformLocation(m_ID, name));
    if (loc == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_uniformLocationCache[name] = loc; 
    return loc;
}

unsigned int Shader::getID() {
    return m_ID;
}

void Shader::use() {
    GL_CALL(glUseProgram(m_ID));
}

void Shader::uniform(const char* name, bool value) {
    GL_CALL(glUniform1i(m_getUniformLocation(name), (int)value)); 
}
void Shader::uniform(const char* name, int value) {
    GL_CALL(glUniform1i(m_getUniformLocation(name), value)); 
}
void Shader::uniform(const char* name, float value) {
    GL_CALL(glUniform1f(m_getUniformLocation(name), value));
}
void Shader::uniform(const char* name, int v1, int v2) {
    GL_CALL(glUniform2i(m_getUniformLocation(name), v1, v2));
}
void Shader::uniform(const char* name, int v1, int v2, int v3) {
    GL_CALL(glUniform3i(m_getUniformLocation(name), v1, v2, v3));
}
void Shader::uniform(const char* name, int v1, int v2, int v3, int v4) {
    GL_CALL(glUniform4i(m_getUniformLocation(name), v1, v2, v3, v4));
}
void Shader::uniform(const char* name, float v1, float v2) {
    GL_CALL(glUniform2f(m_getUniformLocation(name), v1, v2));
}
void Shader::uniform(const char* name, float v1, float v2, float v3) {
    GL_CALL(glUniform3f(m_getUniformLocation(name), v1, v2, v3));
}
void Shader::uniform(const char* name, float v1, float v2, float v3, float v4) {
    GL_CALL(glUniform4f(m_getUniformLocation(name), v1, v2, v3, v4));
}
void Shader::uniform(const char* name, glm::mat4 transform) {
    GL_CALL(glUniformMatrix4fv(m_getUniformLocation(name) , 1, GL_FALSE, glm::value_ptr(transform)));
}
void Shader::uniform(const char* name, glm::vec4 transform) {
    GL_CALL(glUniform4fv(m_getUniformLocation(name) , 1, glm::value_ptr(transform)));
}



