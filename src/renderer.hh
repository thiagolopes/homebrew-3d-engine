#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "buffers.hh"
#include "shaders.hh"
#include "imgui.h"

void GL_debug_clear_error();
void GL_debug_chek_error();

class Renderer {
 private:
 public:
  Renderer(bool z_buffer = true, bool frame_cap = true);
  void draw(const VertexArray &va, const IndexBuffer &ib) const;
  void set_depth_test(bool flag = true) const;
  void set_swap_interval(bool flag = true) const;
};

class ImGuiRenderer {
 private:
 public:
  ImGuiRenderer(GLFWwindow *window);
  ~ImGuiRenderer();

  void draw() const;
  void begin_frame() const;
};
