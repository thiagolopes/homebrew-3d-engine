#pragma once

#include "buffers.hh"
#include "shaders.hh"
#include <GL/glew.h>

#define ASSERT(X)                                                                                                      \
  if (!(X))                                                                                                            \
    __builtin_trap();

#define GLAssert(X)                                                                                                    \
  GLClearError();                                                                                                      \
  X;                                                                                                                   \
  ASSERT(GLLogCall(#X, __FILE__, __LINE__))

void GL_debug_clear_error();
void GL_debug_chek_error();

class Renderer {
private:
public:
  void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
  void clear() const;
};
