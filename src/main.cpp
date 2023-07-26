#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "textures.hh"
#include "camera.hh"

#include "vendor/imgui/imgui.h"
#define TAU 6.28

int width = 1280;
int height = 720;

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
// triagle positions and incidices - vertex collection
float positions[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, // A 0 NORMAL A
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f, // B 1 NORMAL A
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f,  -1.0f, // C 2 NORMAL A
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  0.0f,  -1.0f, // D 3 NORMAL A
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,  // E 4 NORMAL B
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  // F 5 NORMAL B
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  // G 6 NORMAL B
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  // H 7 NORMAL B

    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f, // D 8  NORMAL C
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f, // A 9  NORMAL C
    -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, -1.0f, 0.0f,  0.0f, // E 10 NORMAL C
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -1.0f, 0.0f,  0.0f, // H 11 NORMAL C
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, // B 12 NORMAL D
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f, // C 13 NORMAL D
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, // G 14 NORMAL D
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, // F 15 NORMAL D

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, // A 16 NORMAL E
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, // B 17 NORMAL E
    0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,  -1.0f, 0.0f, // F 18 NORMAL E
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, // E 19 NORMAL E
    0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f, // C 20 NORMAL F
    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f, // D 21 NORMAL F
    -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f, // H 22 NORMAL F
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f, // G 23 NORMAL F
};

unsigned int indices[] = {
    // front
    0, 3, 2, 2, 1, 0,
    // back
    4, 5, 6, 6, 7, 4,
    // left
    11, 8, 9, 9, 10, 11,
    //  right
    12, 13, 14, 14, 15, 12,
    // bottom
    16, 17, 18, 18, 19, 16,
    // top
    20, 21, 22, 22, 23, 20};
size_t indicies_len = sizeof(indices) / sizeof(unsigned int);

int main(void) {
  char window_name[] = "Engine3D";

  Renderer render(window_name, width, height);
  render.set_swap_interval(false);
  render.set_depth_test();
  // render.set_mouse_moviment_callback((void *)mouse_handler);
  // render.set_mouse_scroll_callback((void *)scroll_callback);
  // render.set_mouse_button_callback((void *)mouse_button_callback);

  // setup free camera
  Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

  // // draw steps:
  // 1. vertex buffer (in vram; collections of vertex)
  // 2. shadder (describe how to rasterization will render the collecttion of
  // vertex aka vertex buffer)
  // bind VAO (vertex array)
  VertexArray va;
  VertexArray va_light;

  VertexBuffer vb(positions, sizeof(positions));
  IndexBuffer ib(indices, indicies_len);

  VertexBufferLayout vbl;
  vbl.push<float>(3);
  vbl.push<float>(2);
  vbl.push<float>(3);

  va.add_buffer(vb, vbl);
  va_light.add_buffer(vb, vbl);

  // load shader source code
  Shader shader("res/shaders/basic.shader");
  Shader shader_light("res/shaders/light.shader");

  // texture
  Texture texture("res/textures/texture_mine.jpg");
  Texture texture_two("res/textures/texture.png");
  Texture texture_light("res/textures/light.png");
  shader_light.set_uniform1i("u_Texture", 0);

  // imgui
  ImGuiRenderer imgui(render.get_window());
  ImGuiIO &imgui_io = ImGui::GetIO();

  // So, basically MVP:
  // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
  // View matrix: defines position and orientation of the "camera".
  // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj = glm::perspective(glm::radians(camera.get_fov()),
                                    (float)render.get_width() / (float)render.get_height(), 0.1f, 100.0f);

  bool rotate = true;

  // light position
  glm::vec3 light_position(2.0, 1.0, 3.0);
  glm::vec3 light_color(1.0, 1.0, 1.0);
  /* Loop until the user closes the window */
  while (render.running()) {
    imgui.clear();
    render.clear();

    {
      if (glfwGetKey(render.get_window(), GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(camera_direction_t::FORWARD, render.get_deltatime());
      if (glfwGetKey(render.get_window(), GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(camera_direction_t::BACKWARD, render.get_deltatime());
      if (glfwGetKey(render.get_window(), GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(camera_direction_t::LEFT, render.get_deltatime());
      if (glfwGetKey(render.get_window(), GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(camera_direction_t::RIGHT, render.get_deltatime());
    }

    {
      ImGui::Begin("Menu");
      ImGui::Checkbox("Rotate?", &rotate);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui_io.Framerate, imgui_io.Framerate);
      ImGui::End();
    }

    /* draw here */
    view = camera.get_camera_matrix();

    // first cube
    {
      shader.bind();
      texture.bind();
      shader.set_uniform1i("u_Texture", 0);
      shader.set_uniform_mat4("u_V", view);

      model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 1.0, -1.0)); // multiply z to use positive bar in gui
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      if (rotate) {
        model = glm::rotate(model, render.get_time() * glm::radians(50.0f), glm::vec3(-0.5f, -1.0f, -1.0f));
      };

      shader.set_uniform_vec3("u_LightColor", light_color);
      shader.set_uniform_vec3("u_LightPos", light_position);
      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader);

      texture.unbind();
      shader.unbind();
    }

    // seconde cube
    {
      shader.bind();
      texture_two.bind();
      shader.set_uniform1i("u_Texture", 0);
      shader.set_uniform_mat4("u_V", view);

      model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0, 1.0, -3.0));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

      if (rotate) {
        model = glm::rotate(model, render.get_time() * glm::radians(50.0f), glm::vec3(0.5f, 5.0f, 1.0f));
      };

      shader.set_uniform_vec3("u_LightColor", light_color);
      shader.set_uniform_vec3("u_LightPos", light_position);
      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader); // todo: movo to a batch render and draw once;

      texture_two.unbind();
      shader.unbind();
    }

    // third cube
    {
      shader_light.bind();
      texture_light.bind();
      shader_light.set_uniform1i("u_Texture", 0);
      shader_light.set_uniform_mat4("u_V", view);

      model = glm::translate(glm::mat4(1.0f), light_position);
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

      shader_light.set_uniform_mat4("u_M", model);
      shader_light.set_uniform_mat4("u_P", proj);

      render.draw(va_light, ib, shader_light); // todo: movo to a batch render and draw once;
      texture_light.unbind();
      shader_light.unbind();
    }

    imgui.draw();
    render.end_frame();
  }

  return 0;
}
