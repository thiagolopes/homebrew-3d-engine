#include "textures.hh"
#include <cstdlib>
#include <iostream>

#define GLM_FORCE_XYZW_ONLY
#include "renderer.hh"
#include "window.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "camera.hh"
#include "mesh.hh"
#include "lights.hh"
#include "models.hh"
#include "materials.hh"
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

  Renderer render(true, true);
  ImGuiRenderer ui(win.get_window());

  Shader shader("shaders/material.shader");
  Shader shader_light("shaders/light.shader");
  ShaderUniform u_material(shader);
  ShaderUniform u_dir_light(shader_light);

  PointLight pl(4.0f, 0.23f, 1.0f, 0.0f, 0.6f, 0.028f, 0.0035f);
  DirLight dl(-0.2f, 0.0f, 0.0f, 0.0f);

  Mesh cube_mesh(Container::vertices, Container::indices);
  Mesh plane_mesh(Plane::vertices, Plane::indices);
  Texture container_texture("res/textures/container.png", true); //let it repeat
  Model rock_model("rock");
  Model earth_model("planet");
  Model cyborg_model("cyborg");

  Entity cyborg1(cyborg_model, 8.0f, .6f, 0.4f);
  Entity cyborg2(cyborg_model, -5.0f, .5f, -0.3f);
  Entity cyborg3(cyborg_model, 11.0f, .3f, 0.0f);
  
  Entity earth(earth_model, -1.0f, 2.0f, 0.0f);
  Entity cube(&cube_mesh, nullptr, 3.0f, 5.0f, 0.0f);
  Entity plane_container(&plane_mesh, nullptr, 10.0f, 5.0f, 0.0f);

  earth_model.material->set_emissioness(1.0f);
  earth.scale(.5f, .5f, .5f);
  plane_container.scale(1);

  /* Loop until the user closes the window */
  while (win.running())
    {
      win.begin_frame();

      ui.begin_frame();
      ui.debug(pl);
      ui.debug(dl);

      camera.update(win.get_deltatime());

      //earth
      shader.bind();
      earth.inc_angle(0.1);
      u_material.setup_uniforms(earth.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, &dl);
      earth_model.draw(render, shader); // TODO update to be render handler: render.draw();

      // plane
      shader.bind();
      container_texture.bind();
      u_material.setup_uniforms(plane_container.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, &dl);
      plane_mesh.draw(render, shader);

      // cyborgs
      shader.bind();
      u_material.setup_uniforms(cyborg1.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, &dl);
      cyborg_model.draw(render, shader);
      shader.bind();
      u_material.setup_uniforms(cyborg2.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, &dl);
      cyborg_model.draw(render, shader);
      shader.bind();
      u_material.setup_uniforms(cyborg3.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, &dl);
      cyborg_model.draw(render, shader);
      
      //light point
#if 1
      shader_light.bind();
      cube.position(pl.position.x, pl.position.y, pl.position.z);
      u_dir_light.setup_uniforms(cube.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl, nullptr);
      cube_mesh.draw(render, shader_light);
#endif

      ui.draw();
      win.end_frame();

    }

  return EXIT_SUCCESS;
}
