#pragma once
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

class MouseEvents
{
private:
public:
  MouseEvents(void);
  ~MouseEvents();
  void callback_glfw_scroll(GLFWwindow *window, double xoffset, double yoffset);
  void callback_glfw_mouse(GLFWwindow *window, double xposIn, double yposIn);

  glm::vec2 get_position();
  bool get_state(const int key);
};

class KeyEvents
{
private:
public:
  KeyEvents(void);
  ~KeyEvents();
  void callback_glfw(GLFWwindow *window, int key, int scancode, int action, int mods);
  bool get_state(const int key);
};
