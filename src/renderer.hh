#pragma once

#include <GL/glew.h>

#define ASSERT(X)                                                                                                      \
  if (!(X))                                                                                                            \
    __builtin_trap();

#define GLAssert(X)                                                                                                    \
  GLClearError();                                                                                                      \
  X;                                                                                                                   \
  ASSERT(GLLogCall(#X, __FILE__, __LINE__))

void
GL_debug_clear_error();
void
GL_debug_chek_error();
