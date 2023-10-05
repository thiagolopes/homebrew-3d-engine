#include "opengl_middleware.hh"
#include <cstdlib>
#include <iostream>

void
viewport_size_callback(GLFWwindow *window, int width, int height)
{
  std::printf("called\n");
  glViewport(0, 0, width, height);
}

GLFWwindow *
OpenGLCallback::get_window()
{
  OpenGLCallback &i = get_instance();
  if (i._win == nullptr)
    {
      std::cerr << "Set window before use" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  return i._win;
}

void
OpenGLCallback::middleware_viewport()
{
  glfwSetFramebufferSizeCallback(get_window(), (GLFWframebuffersizefun)viewport_size_callback);
}
