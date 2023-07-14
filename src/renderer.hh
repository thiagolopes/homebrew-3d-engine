#pragma once

#include "buffers.hh"
#include "shaders.hh"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"

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

class ImGuiRenderer {
private:
public:
  ImGuiRenderer(GLFWwindow *window);
  ~ImGuiRenderer();

  void draw() const;
  void clear() const;
};
