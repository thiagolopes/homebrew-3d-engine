#include <cstdio>
#include <cstdlib>
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
#include "inputs.hh"

int
main(void)
{
  int width = 1280;
  int height = 720;
  char window_name[] = "Engine3D";
  Window win(window_name, width, height);

  Camera camera(8.0f, width, height);
  camera.set_moviment_speed(8.0f);

  Renderer render;
  ImGuiRenderer ui(win.get_window());

  Shader shader("shaders/material.shader");
  Shader shader_light("shaders/light.shader");

  PointLight pl(0.0f, 0.23f, 1.0f, 0.0f, 0.6f, 0.028f, 0.0035f);
  DirLight dl(-0.2f, 0.0f, 0.0f, 0.0f);

  Mesh cube_mesh(Container::vertices, Container::indices);
  Model rock_model("rock");
  Model earth_model("sphere");

  Entity earth(earth_model, 1.0f, 2.0f, 0.0f);
  Entity cube(&cube_mesh, nullptr, 3.0f, 1.0f, 0.0f);

  Keyboard &kb = Keyboard::get_instance();
  Mouse &mouse = Mouse::get_instance();

  earth_model.material->set_emissioness(1.0f);

  /* Loop until the user closes the window */
  while (win.running())
    {
      win.begin_frame();

      ui.begin_frame();
      ui.debug(pl);
      ui.debug(dl);

      camera.update(kb, mouse, win.get_deltatime());
      // earth.position(space_tile * sin(win.get_time()), 0.0, space_tile * cos(win.get_time()));
      // earth.inc_angle(.5f);

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
        // cube.position(4.0f * sin(win.get_time()), 0.0f, 5.0f * cos(win.get_time()));
        shader_light.set_MVP(cube.get_model_position(), camera.get_view_matrix(), camera.get_projection());

        cube_mesh.draw(render, shader_light); // todo: movo to a batch render and draw once;
      }
#endif

      ui.draw();
      win.end_frame();
    }

  return EXIT_SUCCESS;
}
