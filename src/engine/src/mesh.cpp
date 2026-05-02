
#include "mesh.hpp"
#include <glad/glad.h>
#include <cmath>
#include "logging.hpp"

Mesh::Mesh(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int vertexCount) {
    m_VAO = vao;
    m_VBO = vbo;
    m_EBO = ebo;
    m_vertexCount = vertexCount;
}

Mesh::~Mesh() {
    GL_CALL(glDeleteBuffers(1, &m_VBO));
    GL_CALL(glDeleteBuffers(1, &m_EBO));
    GL_CALL(glDeleteVertexArrays(1, &m_VAO));
}

void Mesh::bind() const {
    GL_CALL(glBindVertexArray(m_VAO));
}

void Mesh::unbind() const {
    GL_CALL(glBindVertexArray(0));
}

unsigned int Mesh::getVertexCount() const {
    return m_vertexCount;
}
