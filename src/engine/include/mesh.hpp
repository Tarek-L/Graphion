
#pragma once

class Mesh {
private:
    unsigned int m_VAO, m_VBO, m_EBO, m_vertexCount;
public: 
    Mesh(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int vertexCount);

    ~Mesh();

    void bind() const;
    void unbind() const;

    unsigned int getVertexCount() const;

};
