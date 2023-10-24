#pragma once
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

class OpenGLCallback
{
private:
  GLFWwindow *_win;
  void middleware_viewport();
  void middleware_keyboard();
  void middleware_mouse();
  GLFWwindow *get_window();

public:
  inline static OpenGLCallback &
  get_instance()
  {
    static OpenGLCallback instance;
    return instance;
  }
  static void
  set_window(GLFWwindow *win)
  {
    OpenGLCallback &i = get_instance();
    i._win = win;
    i.middleware_viewport();
    i.middleware_keyboard();
    i.middleware_mouse();
  };
};
