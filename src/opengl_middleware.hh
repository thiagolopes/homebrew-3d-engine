#pragma once
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

class OpenGLCallback
{
private:
  GLFWwindow *_win;
  void middleware_viewport();
  GLFWwindow *get_window();

public:
  static OpenGLCallback &
  get_instance()
  {
    static OpenGLCallback instance;
    return instance;
  }
  // static void viewport_size_callback(GLFWwindow *window, int width, int height);
  static void
  set_window(GLFWwindow *win)
  {
    OpenGLCallback &i = get_instance();
    i._win = win;
    i.middleware_viewport();
  };
};
