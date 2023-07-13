#include "renderer.hh"
#include <iostream>

void GL_debug_clear_error() {
  while (glGetError() != GL_NO_ERROR) {
  };
}

void GL_debug_chek_error() {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
  }
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
  shader.bind();
  va.bind();
  ib.bind();

  GL_debug_clear_error();
  glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
  GL_debug_chek_error();
};

void Renderer::clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
};
