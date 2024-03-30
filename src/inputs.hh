#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

#define GLFW_A_KEYCODE 65
enum Key {
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
    Length, // 'hack' to keep track of enum length
};

class Keyboard {
  private:
    std::unordered_map<Key, bool> state;
    Keyboard(void);

  public:
    static Keyboard &get_instance() {
        static Keyboard instance;
        return instance;
    }
    bool get_state(const int key);
    void set_state(const int key, const bool action);
    bool key_in_bounds(int key);
};

struct MousePosition {
    float x;
    float y;
};

enum MouseKey {
    LEFT,
    RIGHT,
};

class Mouse {
  private:
    Mouse(void);
    std::unordered_map<MouseKey, bool> state;
    MousePosition last_position;

  public:
    MousePosition pointer_position;
    float scroll_y;

    static Mouse &get_instance() {
        static Mouse instance;
        return instance;
    }

    MousePosition get_last_position();
    MousePosition get_diff_moviment();
    void set_state(const int key, const bool action);
    void update_position(float x, float y);
    void next_frame();
    bool get_state(const MouseKey key);
    bool is_moving();
};
