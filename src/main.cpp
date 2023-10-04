#include <iostream>
#include <math.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#define GLM_FORCE_XYZW_ONLY
#include "glm/gtc/matrix_transform.hpp"
#include "renderer.hh"
#include "window.hh"
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

int
main(void)
{
  char window_name[] = "Engine3D";
  camera.set_moviment_speed(8.0f);

  Window win(window_name, width, height);
  win.set_mouse_moviment_callback((void *)mouse_callback);
  win.set_mouse_scroll_callback((void *)scroll_callback);
  win.set_viewport_size_callback((void *)viewport_size_callback);

  Renderer render;
  ImGuiRenderer imgui(win.get_window());

  // used matrix instances;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;

  // light
  Shader shader("shaders/material.shader");
  Shader shader_light("shaders/light.shader");
  PointLight pl(0.0f, 0.23f, 1.0f, 0.0f, 0.6f, 0.028f, 0.0035f);
  DirLight dl(-0.2f, 0.0f, 0.0f, 0.0f);
  Mesh light(Container::vertices, Container::indices);
  Model rock_model("rock");
  Model earth_model("sphere");
  Entity earth(earth_model, 1.0f, 2.0f, 0.0f);

  earth_model.material->set_emissioness(1.0f);
  int space_tile = 8;

  /* Loop until the user closes the window */
  while (win.running())
    {
      imgui.begin_frame();
      win.begin_frame();

      // handler key
      camera.process_keyboard((camera_direction_t)win.get_key_pressed(), win.get_deltatime());

      // imgui panel
      // TODO move to imgui_render.debug(pl); imgui_render.debug(dl);
      imgui.debug(pl);
      imgui.debug(dl);

      earth.position(space_tile * sin(win.get_time()), 0.0, space_tile * cos(win.get_time()));
      earth.inc_angle(.5f);

      view = camera.get_view_matrix();
      model = earth.get_model_position();
      proj = camera.get_projection();

      shader.bind();
      shader.set_MVP(model, view, proj);
      shader.set_point_light(pl);
      shader.set_directional_light(dl);

      // render.draw();
      earth_model.draw(render, shader);
#if 1
      // third cube - the light
      {
        shader_light.bind();
        shader_light.set_uniform_mat4("u_V", view);

        shader_light.set_uniform_vec3("u_Light.ambient", pl.ambient);
        shader_light.set_uniform_vec3("u_Light.diffuse", pl.diffuse);

        pl.position.x = 4.0f * sin(win.get_time());
        pl.position.z = 5.0f * cos(win.get_time());

        model = glm::translate(glm::mat4(1.0f), pl.position);
        model = glm::scale(model, glm::vec3(0.5));

        shader_light.set_uniform_mat4("u_M", model);
        shader_light.set_uniform_mat4("u_P", proj);

        light.draw(render, shader_light); // todo: movo to a batch render and draw once;
      }
#endif

      imgui.draw();
      win.end_frame();
    }

  return 0;
}

void
mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (first_mouse_call)
    {
      mouse_last_x = xpos;
      mouse_last_y = ypos;
      first_mouse_call = false;
    }

  float xoffset = xpos - mouse_last_x;
  float yoffset = mouse_last_y - ypos; // reversed since y-coordinates go from bottom to top

  mouse_last_x = xpos;
  mouse_last_y = ypos;

  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  if (state)
    {
      camera.process_mouse_moviment(xoffset, yoffset);
    }
}

void
scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.process_mouse_scroll(yoffset);
}
void
viewport_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
