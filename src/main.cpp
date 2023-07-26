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
// triagle positions and incidices - vertex collection
float positions[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // A 0
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // B 1
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // C 2
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // D 3
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // E 4
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // F 5
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // G 6
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // H 7

    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, // D 8
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // A 9
    -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, // E 10
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // H 11
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, // B 12
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, // C 13
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // G 14
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, // F 15

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // A 16
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // B 17
    0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, // F 18
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, // E 19
    0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, // C 20
    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, // D 21
    -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, // H 22
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // G 23
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
  int width = 1280;
  int height = 720;
  char window_name[] = "Engine3D";

  Renderer render(window_name, width, height);
  render.set_swap_interval();
  render.set_depth_test();
  // render.set_mouse_moviment_callback((void *)mouse_handler);
  // render.set_mouse_scroll_callback((void *)scroll_callback);
  // render.set_mouse_button_callback((void *)mouse_button_callback);

  Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

  // // draw steps:
  // 1. vertex buffer (in vram; collections of vertex)
  // 2. shadder (describe how to rasterization will render the collecttion of
  // vertex aka vertex buffer)
  // bind VAO (vertex array)
  VertexArray va;
  VertexBuffer vb(positions, sizeof(positions));
  IndexBuffer ib(indices, indicies_len);

  VertexBufferLayout vbl;
  vbl.push<float>(3);
  vbl.push<float>(2);

  va.add_buffer(vb, vbl);

  // load shader source code
  Shader shader("res/shaders/basic.shader");
  shader.bind();

  // texture
  Texture texture("res/textures/texture_mine.jpg");
  Texture texture_two("res/textures/texture.png");
  texture.bind();
  shader.set_uniform1i("u_Texture", 0);

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

  glm::vec3 rotateA(1.0f, 1.0f, 1.0f);
  glm::vec3 rotateB(1.0f, 1.0f, 1.0f);

  glm::vec3 translationA(0, 0, 0);
  glm::vec3 translationB(3, 3, 3);

  float angleA = 0;
  float angleB = 1;
  float scaleA = 1.0;
  float scaleB = 2.5;

  bool rotate = true;
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
      ImGui::Begin("OpenGL MVP");
      ImGui::SliderFloat("Angle A", &angleA, 0.0f, TAU);
      ImGui::SliderFloat3("RotationA", &rotateA.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Translation A", &translationA.x, -100.0f, 100.0f);
      ImGui::SliderFloat("Scale A", &scaleA, 0.0f, 300.0f);
      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ImGui::SliderFloat("Angle B", &angleB, 0.0f, TAU);
      ImGui::SliderFloat3("RotationB", &rotateB.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Translation B", &translationB.x, -100.0f, 100.0f);
      ImGui::SliderFloat("Scale B", &scaleB, 0.0f, 300.0f);
      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ImGui::Checkbox("Rotate?", &rotate);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui_io.Framerate, imgui_io.Framerate);
      ImGui::End();
    }

    /* draw here */
    texture.bind();
    view = camera.get_camera_matrix();
    shader.set_uniform_mat4("u_V", view);
    {
      // first cube
      model = glm::translate(glm::mat4(1.0f),
                             translationA * glm::vec3(1.0, 1.0, -1.0)); // multiply z to use positive bar in gui
      model = glm::rotate(model, angleA, rotateA);
      model = glm::scale(model, glm::vec3(scaleA, scaleA, scaleA));

      if (rotate) {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      };
      // camera
      // view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader);
    }
    texture.unbind();

    // change texture
    texture_two.bind();
    {
      // second cube

      model = glm::translate(glm::mat4(1.0f), translationB * glm::vec3(1.0, 1.0, -1.0));
      model = glm::rotate(model, angleB, rotateB);
      model = glm::scale(model, glm::vec3(scaleB, scaleB, scaleB));

      if (rotate) {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      };

      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader); // todo: movo to a batch render and draw once;
    }
    texture_two.unbind();

    imgui.draw();
    render.end_frame();
  }
  return 0;
}
