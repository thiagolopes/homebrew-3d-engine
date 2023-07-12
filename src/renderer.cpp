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
