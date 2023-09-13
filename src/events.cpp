#include "events.hh"
#include <GLFW/glfw3.h>
#include "vector"

std::vector<bool> KEYS_STATUS(1024, false);
std::vector<bool> MOUSE_STATUS(2, false);
std::vector<float> MOUSE_POSITION(2, 0.0);

bool KeyEvents::get_state(const int key) { return KEYS_STATUS[key]; }

void KeyEvents::callback_glfw(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        KEYS_STATUS[key] = true;
    }
    if (action == GLFW_RELEASE) {
        KEYS_STATUS[key] = false;
    }
}
