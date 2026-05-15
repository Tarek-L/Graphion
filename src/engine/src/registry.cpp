
//registry.cpp

#include "registry.hpp"
#include "logging.hpp"
#include <cstring>

#define PI 3.1415926535
Mesh* Registry::getMesh(const char* name) {
    if (m_Meshes.find(name) != m_Meshes.end())
        return m_Meshes[name].get();
    if (0 == std::strcmp(name, "square"))
        return m_createSquare();
    if (0 == std::strcmp(name, "circle"))
        return m_createCircle();
    if (0 == std::strcmp(name, "triangle"))
        return m_createTriangle();


    if (m_Meshes.find("error") != m_Meshes.end())
        return m_Meshes["error"].get();
    return m_createErrorTriangle();    
    
}

Shader* Registry::getShader(const char* name) {
    if (m_Shaders.find(name) != m_Shaders.end())
        return m_Shaders[name].get();
    if (0 == std::strcmp(name, "basic")) {

        m_Shaders["basic"] = std::make_unique<Shader>("../../assets/shaders/basic.vert", "../../assets/shaders/basic.frag");
        return m_Shaders["basic"].get();
    }
    return nullptr;
}

Mesh* Registry::m_createSquare() {
    float vertices[] = {
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3};
    
    unsigned int VAO, VBO, EBO;

    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glGenBuffers(1, &EBO));

    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));
        
    GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
         
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    m_Meshes["square"] = std::make_unique<Mesh>(VAO, VBO, EBO, 6);

    return m_Meshes["square"].get();
}

Mesh* Registry::m_createTriangle() {
    float vertices[] = {
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
         0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = { 0, 1, 2};

    unsigned int VAO, VBO, EBO;

    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glGenBuffers(1, &EBO));

    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));
        
    GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
        
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    m_Meshes["triangle"] = std::make_unique<Mesh>(VAO, VBO, EBO, 3);

    return m_Meshes["triangle"].get();
}

Mesh* Registry::m_createErrorTriangle() {
    float vertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2};

    unsigned int VAO, VBO, EBO;

    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glGenBuffers(1, &EBO));

    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    m_Meshes["error"] = std::make_unique<Mesh>(VAO, VBO, EBO, 3);

    return m_Meshes["error"].get();
}

Mesh* Registry::m_createCircle() {
    float vertices[637];

    unsigned int indices[270];
    
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f;
    vertices[4] = 1.0f;
    vertices[5] = 1.0f;
    vertices[6] = 1.0f;

    for(int i = 1; i < 90; ++i) {
        vertices[7 * i] = 0.5f * cos(i * 4 * PI / 180);
        vertices[7 * i + 1] = 0.5f * sin(i * 4 * PI / 180);
        vertices[7 * i + 2] = 0.0f;
        vertices[7 * i + 3] = 1.0f;
        vertices[7 * i + 4] = 1.0f;
        vertices[7 * i + 5] = 1.0f;
        vertices[7 * i + 6] = 1.0f;
        indices[3 * i - 3] = 0;
        indices[3 * i - 2] = i;
        indices[3 * i - 1] = i + 1;
    }
        vertices[7 * 90] = 0.5f * cos(90 * 4 * PI / 180);
        vertices[7 * 90 + 1] = 0.5f * sin(90 * 4 * PI / 180);
        vertices[7 * 90 + 2] = 0.0f;
        vertices[7 * 90 + 3] = 1.0f;
        vertices[7 * 90 + 4] = 1.0f;
        vertices[7 * 90 + 5] = 1.0f;
        vertices[7 * 90 + 6] = 1.0f;
        indices[3 * 90 - 3] = 0;
        indices[3 * 90 - 2] = 90;
        indices[3 * 90 - 1] = 1;

    
    unsigned int VAO, VBO, EBO;

    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glGenBuffers(1, &EBO));

    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));
        
    GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
        
        
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    m_Meshes["circle"] = std::make_unique<Mesh>(VAO, VBO, EBO, 270);

    return m_Meshes["circle"].get();

}



