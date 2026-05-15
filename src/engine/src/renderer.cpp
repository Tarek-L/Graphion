
//renderer.cpp

#include "renderer.hpp"
#include "camera.hpp"
 
#include <glad/glad.h>
#include <logging.hpp>
 

void Renderer::init()
{
    // Standard alpha blending: src_alpha blends over 1-src_alpha background
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
 
    // Depth testing — required even for 2D when entities overlap
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glDepthFunc(GL_LESS));
 
    LOG_INFO("Renderer initialized");
}
 

void Renderer::draw(const Entity& entity)
{
    Mesh*   mesh   = entity.getMesh();
    Shader* shader = entity.getShader();
 
    ASSERT(mesh,   "Renderer::draw — entity has a null Mesh");
    ASSERT(shader, "Renderer::draw — entity has a null Shader");
 
    shader->use();
 
    shader->uniform("uView",       Camera::getViewMatrix());
    shader->uniform("uProjection", Camera::getProjectionMatrix());
    shader->uniform("uModel",      entity.transform.getModelMatrix());
    shader->uniform("uColor",      entity.color);
 
    mesh->bind();
    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        mesh->getVertexCount(),
        GL_UNSIGNED_INT,
        nullptr
    ));
}
 
