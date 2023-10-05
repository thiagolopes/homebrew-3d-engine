#include "events.hh"
#include <GLFW/glfw3.h>
#include <cstdio>

Keyboard::Keyboard()
{
  for (size_t i = GLFW_A_KEYCODE; i < Key::Length; i++)
    state[(Key)i] = false;
}

void
Keyboard::glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key > GLFW_A_KEYCODE && key < Key::Length)
    {
    get_instance().state[(Key)key] = action;
    }
}

bool
Keyboard::get_state(const int key)
{
  if (key > GLFW_A_KEYCODE && key < Key::Length)
    return get_instance().state[(Key)key];
  return false;
}
