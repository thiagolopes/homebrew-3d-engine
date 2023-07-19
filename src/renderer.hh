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
  GLFWwindow *m_window;
  int m_width;
  int m_height;
  float m_deltatime;
  float m_lastframe;

  inline void update_deltatime_frame() {
    float current_frame = get_time();
    m_deltatime = current_frame - m_lastframe;
    m_lastframe = current_frame;
  }

public:
  Renderer(char *window_name, float width, float height);
  ~Renderer();
  void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
  void clear() const;
  void end_frame();
  void set_depth_test(bool flag = true) const;
  void set_swap_interval(bool flag = true) const;
  float get_time();
  bool running();
  void set_mouse_moviment_callback(void *f);
  void set_mouse_scroll_callback(void *f);

  inline GLFWwindow *get_window() const { return m_window; };
  inline int get_width() const { return m_width; };
  inline int get_height() const { return m_height; };
  inline float get_deltatime() { return m_deltatime; };
};

class ImGuiRenderer {
private:
public:
  ImGuiRenderer(GLFWwindow *window);
  ~ImGuiRenderer();

  void draw() const;
  void clear() const;
};
