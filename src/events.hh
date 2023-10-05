#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

#define GLFW_A_KEYCODE 65
enum Key
{
  A = GLFW_A_KEYCODE, // 65 to keep GLFW compatiblity
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  Length, // 'hack to keep track of enum length'
};

class Keyboard
{
private:
  std::unordered_map<Key, bool> state;
  Keyboard(void);

public:
  static Keyboard &
  get_instance()
  {
    static Keyboard instance;
    return instance;
  }
  void update();
  bool get_state(const int key);
  static bool
  key_in_bounds(int key)
  {
    if (key >= Key::A && key <= Key::Length)
      {
        return true;
      }
    return false;
  }
  static void glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

enum MouseKey
{
  LBUTTON,
  RBUTTON
};

class Mouse
{
private:
public:
  Mouse(void);
  ~Mouse();

  bool get_state(const int key);
};
