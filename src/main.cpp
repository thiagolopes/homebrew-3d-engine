#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#define GLM_FORCE_XYZW_ONLY
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
#include "events.hh"


// settings
int width = 1280;
int height = 720;
bool first_mouse_call = true;
float mouse_last_x = width / 2.0f;
float mouse_last_y = height / 2.0f;

// setup free camera
Camera camera(8.0f, width, height);

void viewport_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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

  Shader shader("shaders/material.shader");
  Shader shader_light("shaders/light.shader");

  PointLight pl(0.0f, 0.23f, 1.0f, 0.0f, 0.6f, 0.028f, 0.0035f);
  DirLight dl(-0.2f, 0.0f, 0.0f, 0.0f);

  Mesh cube_mesh(Container::vertices, Container::indices);
  Model rock_model("rock");
  Model earth_model("sphere");

  Entity earth(earth_model, 1.0f, 2.0f, 0.0f);
  Entity cube(&cube_mesh, nullptr);

  earth_model.material->set_emissioness(1.0f);
  int space_tile = 8;

  Keyboard &kb = Keyboard::get_instance();
  glfwSetKeyCallback(win.get_window(), (GLFWkeyfun)kb.glfw_callback);

  /* Loop until the user closes the window */
  while (win.running())
    {
      printf("stado do w: %i\n", kb.get_state(Key::W));
      win.begin_frame();

      imgui.begin_frame();
      imgui.debug(pl);
      imgui.debug(dl);

      // handler key
      camera.process_keyboard((camera_direction_t)win.get_key_pressed(), win.get_deltatime());
      camera.update();

      earth.position(space_tile * sin(win.get_time()), 0.0, space_tile * cos(win.get_time()));
      earth.inc_angle(.5f);

      shader.bind();
      shader.set_point_light(pl);
      shader.set_directional_light(dl);
      shader.set_MVP(earth.get_model_position(), camera.get_view_matrix(), camera.get_projection());

      // render.draw();
      earth_model.draw(render, shader);
#if 1
      {
        shader_light.bind();

        shader_light.set_point_light(pl);
        cube.position(4.0f * sin(win.get_time()), 0.0f, 5.0f * cos(win.get_time()));
        shader_light.set_MVP(cube.get_model_position(), camera.get_view_matrix(), camera.get_projection());

        cube_mesh.draw(render, shader_light); // todo: movo to a batch render and draw once;
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
