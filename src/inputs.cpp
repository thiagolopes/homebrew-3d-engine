#include "inputs.hh"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <mutex> // std::mutex

Keyboard::Keyboard()
{
  for (size_t i = Key::A; i < Key::Length; i++)
    state[(Key)i] = false;
}

bool
Keyboard::get_state(const int key)
{
  if (key_in_bounds(key))
    return get_instance().state[(Key)key];
  return false;
}

void
Keyboard::set_state(const int key, const bool action)
{
  if (key_in_bounds(key))
    {
      state[(Key)key] = action;
    }
}

bool
Keyboard::key_in_bounds(int key)
{
  if (key >= Key::A && key <= Key::Length)
    {
      return true;
    }
  return false;
};

Mouse::Mouse()
{
  state[MouseKey::LEFT] = false;
  state[MouseKey::RIGHT] = false;
}

bool
Mouse::get_state(MouseKey key)
{
  return state[key];
}

bool
Mouse::is_moving()
{
  MousePosition diff = get_last_position();
  if (diff.x == 0 && diff.y == 0)
    {
      return false;
    }
  return true;
}

void
Mouse::update_position(float x, float y)
{
  last_position.x = pointer_position.x;
  last_position.y = pointer_position.y;

  pointer_position.x = x;
  pointer_position.y = y;
}

void
Mouse::next_frame()
{
  last_position.x = pointer_position.x;
  last_position.y = pointer_position.y;
  scroll_y = 0;
}

MousePosition
Mouse::get_last_position()
{
  return last_position;
}

MousePosition
Mouse::get_diff_moviment()
{
  return MousePosition{ pointer_position.x - last_position.x, last_position.y - pointer_position.y };
}

void
Mouse::set_state(const int key, const bool action)
{
  state[(MouseKey)key] = action;
}
