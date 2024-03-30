#include <cstdlib>
#include <iostream>

#define GLM_FORCE_XYZW_ONLY
#include "window.hh"
#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "inputs.hh"
#include "camera.hh"
#include "mesh.hh"
#include "textures.hh"
#include "materials.hh"
#include "lights.hh"
#include "models.hh"
#include "entity.hh"
#include "containers.hh"

#define W 1280
#define H 720

int main(void) {
    int width = W;
    int height = H;
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
    Texture container_texture("res/textures/container.png", true); // let it repeat
    Model planet_model("planet");
    Model cyborg_model("cyborg");

    Entity cyborg1(cyborg_model, 8.0f, .6f, 0.4f);
    Entity cyborg2(cyborg_model, -5.0f, .5f, -0.3f);
    Entity cyborg3(cyborg_model, 11.0f, .3f, 0.0f);

    Entity planet(planet_model, -1.0f, 2.0f, 0.0f);
    Entity cube(&cube_mesh, nullptr, 3.0f, 5.0f, 0.0f);
    Entity plane_container(&plane_mesh, nullptr, 10.0f, 5.0f, 0.0f);

    planet_model.get_material()->set_emissioness(1.0f);
    planet.scale(.5f, .5f, .5f);
    plane_container.scale(1);
    plane_container.set_rotation_dir(-.7f, 1.0f, 0.5f);

    /* Loop until the user closes the window */
    while (win.running())
    {
        win.begin_frame();

        ui.begin_frame();
        ui.debug(pl);
        ui.debug(dl);

        camera.update(win.get_deltatime());

        // NOTE maybe do a class to organize render
        // by material, to avoid repeat binds?
        // should more investigation;

        // earth
        shader.bind();
        planet_model.material->bind(shader);
        planet.inc_angle(0.1);
        u_material.setup_uniforms(planet.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl,
                                  &dl);
        render.draw_model(planet_model);

        // plane
        shader.bind();
        container_texture.bind();
        plane_container.inc_angle(-.1f);
        u_material.setup_uniforms(plane_container.get_model_position(), camera.get_view_matrix(),
                                  camera.get_projection(), pl, &dl);
        render.draw_mesh(plane_mesh);

        // cyborgs
        shader.bind();
        cyborg_model.material->bind(shader);
        u_material.setup_uniforms(cyborg1.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl,
                                  &dl);
        render.draw_model(cyborg_model);
        shader.bind();
        cyborg_model.material->bind(shader);
        u_material.setup_uniforms(cyborg2.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl,
                                  &dl);
        render.draw_model(cyborg_model);
        shader.bind();
        cyborg_model.material->bind(shader);
        u_material.setup_uniforms(cyborg3.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl,
                                  &dl);
        render.draw_model(cyborg_model);

        // light point
#if 1
        shader_light.bind();
        cube.position(pl.position.x, pl.position.y, pl.position.z);
        u_dir_light.setup_uniforms(cube.get_model_position(), camera.get_view_matrix(), camera.get_projection(), pl,
                                   nullptr);
        render.draw_mesh_wired(cube_mesh);
#endif

        ui.draw();
        win.end_frame();
    }

    return EXIT_SUCCESS;
}
