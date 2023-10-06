#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "inputs.hh"

class Window
{
private:
  GLFWwindow *m_window;
  int m_width;
  int m_height;
  float m_deltatime;
  float m_lastframe;

  inline void
  update_deltatime_frame()
  {
    float current_frame = get_time();
    m_deltatime = current_frame - m_lastframe;
    m_lastframe = current_frame;
  }

public:
  Window(char *window_name, float width, float height);
  ~Window();
  void begin_frame() const;
  void end_frame();
  float get_time();
  bool running();
  void setup(Keyboard keyboard);

  inline GLFWwindow *
  get_window() const
  {
    return m_window;
  };
  inline int
  get_width() const
  {
    return m_width;
  };
  inline int
  get_height() const
  {
    return m_height;
  };
  inline float
  get_deltatime()
  {
    return m_deltatime;
  };
};
