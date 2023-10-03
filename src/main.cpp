#include <iostream>
#include <math.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "glm/gtc/matrix_transform.hpp"
#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "camera.hh"
#include "mesh.hh"
#include "lights.hh"
#include "models.hh"
#include "entity.hh"

#include "containers.hh"

// settings
int width = 1280;
int height = 720;
bool first_mouse_call = true;
float mouse_last_x = width / 2.0f;
float mouse_last_y = height / 2.0f;

// setup free camera
Camera camera(glm::vec3(5.0f, 5.0f, 15.0f), width, height);

void viewport_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void keyboard_handler(Renderer &render);

int main(void) {
  char window_name[] = "Engine3D";
  camera.set_moviment_speed(8.0f);

  Renderer render(window_name, width, height);
  render.set_swap_interval(true);
  render.set_depth_test();

  render.set_mouse_moviment_callback((void *)mouse_callback);
  render.set_mouse_scroll_callback((void *)scroll_callback);
  render.set_viewport_size_callback((void *)viewport_size_callback);

  // load shader source code
  Shader shader("shaders/material.shader");
  Shader shader_light("shaders/light.shader");

  // imgui
  ImGuiRenderer imgui(render.get_window());

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;

  // light
  PointLight pl(0.0f, 0.23f, 1.0f, 0.0f, 0.6f, 0.028f, 0.0035f);
  DirLight dl(-0.2f, 0.0f, 0.0f, 0.0f);
  Mesh light(Container::vertices, Container::indices);
  Model rock_model("rock");
  Model earth_model("sphere");
  Entity earth(earth_model, 1.0f, 2.0f, 0.0f);

  earth_model.material->set_emissioness(1.0f);
  int space_tile = 8;

  /* Loop until the user closes the window */
  while (render.running()) {
    imgui.clear();
    render.clear();

    // handler key
    camera.process_keyboard((camera_direction_t)render.get_key_pressed(), render.get_deltatime());

    // imgui panel
    pl.debug_menu();
    dl.debug_menu();

    earth.position(space_tile * sin(render.get_time()), 0.0, space_tile * cos(render.get_time()));
    earth.inc_angle(.5f);

    view = camera.get_camera_matrix();
    model = earth.word_position();
    proj = camera.get_perspective_view();

    shader.bind();
    shader.set_MVP(model, view, proj);
    shader.set_point_light(pl);
    shader.set_directional_light(dl);

    // move draw to entity
    earth_model.draw(render, shader);
#if 1
    // third cube - the light
    {
      shader_light.bind();
      shader_light.set_uniform_mat4("u_V", view);

      shader_light.set_uniform_vec3("u_Light.ambient", pl.ambient);
      shader_light.set_uniform_vec3("u_Light.diffuse", pl.diffuse);

      pl.position.x = 4.0f * sin(render.get_time());
      pl.position.z = 5.0f * cos(render.get_time());

      model = glm::translate(glm::mat4(1.0f), pl.position);
      model = glm::scale(model, glm::vec3(0.5));

      shader_light.set_uniform_mat4("u_M", model);
      shader_light.set_uniform_mat4("u_P", proj);

      light.draw(render, shader_light);  // todo: movo to a batch render and draw once;
    }
#endif

    imgui.draw();
    render.end_frame();
  }

  return 0;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (first_mouse_call) {
    mouse_last_x = xpos;
    mouse_last_y = ypos;
    first_mouse_call = false;
  }

  float xoffset = xpos - mouse_last_x;
  float yoffset = mouse_last_y - ypos;  // reversed since y-coordinates go from bottom to top

  mouse_last_x = xpos;
  mouse_last_y = ypos;

  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  if (state) {
    camera.process_mouse_moviment(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) { camera.process_mouse_scroll(yoffset); }
void viewport_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
