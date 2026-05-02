
#include "renderer.hpp"
#include "logging.hpp"

void Renderer::clear() const {
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const Entity& entity) const  {
    entity.getShader()->use();
    entity.getShader()->uniform("uTransform", entity.transform.getModelMatrix());
    entity.getShader()->uniform("uColor", entity.color);
    entity.getMesh()->bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, entity.getMesh()->getVertexCount(), GL_UNSIGNED_INT, 0));
}
