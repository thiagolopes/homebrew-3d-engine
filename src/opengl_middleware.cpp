#include "opengl_middleware.hh"
#include "inputs.hh"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <mutex>

void
viewport_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void
glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  Mouse &m = Mouse::get_instance();
  m.scroll_y = yoffset;
};

void
glfw_mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
  Mouse &m = Mouse::get_instance();
  m.update_position((float)xposIn, (float)yposIn);
};

void
glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  Mouse &m = Mouse::get_instance();
  m.set_state((MouseKey)button, action);
};

void
glfw_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  Keyboard &k = Keyboard::get_instance();
  k.set_state(key, action);
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

void
OpenGLCallback::middleware_keyboard()
{
  glfwSetKeyCallback(get_window(), (GLFWkeyfun)glfw_keyboard_callback);
}

void
OpenGLCallback::middleware_mouse()
{
  glfwSetScrollCallback(get_window(), (GLFWscrollfun)glfw_scroll_callback);
  glfwSetCursorPosCallback(get_window(), (GLFWcursorposfun)glfw_mouse_callback);
  glfwSetMouseButtonCallback(get_window(), (GLFWmousebuttonfun)glfw_mouse_button_callback);
};
