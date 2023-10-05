#include "inputs.hh"
#include <GLFW/glfw3.h>
#include <cstdio>

void *
glfw_callback(Keyboard &e)
{
  return (void *)e.glfw_callback;
}

Keyboard::Keyboard()
{
  for (size_t i = Key::A; i < Key::Length; i++)
    state[(Key)i] = false;
}

void
Keyboard::glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key_in_bounds(key))
    {
      get_instance().state[(Key)key] = action;
    }
}

bool
Keyboard::get_state(const int key)
{
  if (key_in_bounds(key))
    return get_instance().state[(Key)key];
  return false;
}
