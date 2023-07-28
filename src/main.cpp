#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "textures.hh"
#include "camera.hh"

#include "vendor/imgui/imgui.h"
#define TAU 6.28

int width = 1280;
int height = 720;
// setup free camera
Camera camera(glm::vec3(0.0f, 0.0f, 12.0f));

struct Material {
  // Texture diffuse;
  float shininess;
};

struct Light {
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

bool first_mouse_call = true;
float mouse_last_x = width / 2.0f;
float mouse_last_y = height / 2.0f;
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
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
  camera.set_moviment_speed(8.0f);

  Renderer render(window_name, width, height);
  render.set_swap_interval(false);
  render.set_depth_test();
  render.set_mouse_moviment_callback((void *)mouse_callback);
  render.set_mouse_scroll_callback((void *)scroll_callback);
  // render.set_mouse_button_callback((void *)mouse_button_callback);

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
  Shader shader("res/shaders/material.shader");
  Shader shader_light("res/shaders/light.shader");

  // texture
  Texture texture_dirty("res/textures/texture_dirty.png");
  Texture texture_dirty_spec("res/textures/texture_dirty_spec.png");
  Texture texture_wood("res/textures/texture_wood.png");
  Texture texture_wood_spec("res/textures/texture_wood_spec.png");
  Texture texture_light("res/textures/light.png");

  // imgui
  ImGuiRenderer imgui(render.get_window());
  ImGuiIO &imgui_io = ImGui::GetIO();

  // So, basically MVP:
  // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
  // View matrix: defines position and orientation of the "camera".
  // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
  bool rotate = true;

  // light
  bool update_color = false;
  Light light = {
      glm::vec3(1.0, 1.0, 2.0),
      glm::vec3(0.4f, 0.4f, 0.4f),
      glm::vec3(0.5f, 0.5f, 0.5f),
      glm::vec3(1.0f, 1.0f, 1.0f),
  };
  // material
  Material material = {
      // TODO add texture ambient
      // TODO add texture specular
      0.7f,
  };

  /* Loop until the user closes the window */
  while (render.running()) {
    imgui.clear();
    render.clear();

    // handler key
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

    // imgui panel
    {
      ImGui::Begin("Menu");
      ImGui::SliderFloat3("Ambiant light", &light.ambient.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Diffuse light", &light.diffuse.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Specular light", &light.specular.x, 0.0f, 1.0f);
      ImGui::SliderFloat("Shininess material", &material.shininess, 0.0f, 1.0f);
      ImGui::Checkbox("Rotate?", &rotate);
      ImGui::Checkbox("Change Color?", &update_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui_io.Framerate, imgui_io.Framerate);
      ImGui::End();
    }

    // color update
    if (update_color) {
      glm::vec3 light_color;
      light_color.x = sin(glfwGetTime() * 2.0f);
      light_color.y = sin(glfwGetTime() * 0.7f);
      light_color.z = sin(glfwGetTime() * 1.3f);

      light.diffuse = light_color * glm::vec3(0.5f);
      light.ambient = light.diffuse * glm::vec3(0.2f);
    }

    /* draw here */
    view = camera.get_camera_matrix();
    proj = glm::perspective(glm::radians(camera.get_fov()), (float)render.get_width() / (float)render.get_height(),
                            0.1f, 100.0f);

    // first cube
    {
      shader.bind();
      texture_dirty.bind();
      texture_dirty_spec.bind(1);
      shader.set_uniform_int1("u_Material.diffuse_texture", 0);
      shader.set_uniform_int1("u_Material.specular_texture", 1);
      shader.set_uniform_mat4("u_V", view);

      model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 1.0, -1.0)); // multiply z to use positive bar in gui
      model = glm::scale(model, glm::vec3(1.0f));
      if (rotate) {
        model = glm::rotate(model, render.get_time() * glm::radians(50.0f), glm::vec3(-0.5f, -1.0f, -1.0f));
      };

      shader.set_uniform_vec3("u_Light.position", light.position);
      shader.set_uniform_vec3("u_Light.ambient", light.ambient);
      shader.set_uniform_vec3("u_Light.diffuse", light.diffuse);
      shader.set_uniform_vec3("u_Light.specular", light.specular);

      shader.set_uniform_float1("u_Material.shininess", material.shininess);

      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader);

      texture_dirty.unbind();
      shader.unbind();
    }

    // seconde cube
    {
      shader.bind();
      texture_wood.bind();
      texture_wood_spec.bind(1);
      shader.set_uniform_int1("u_Material.diffuse_texture", 0);
      shader.set_uniform_int1("u_Material.specular_texture", 1);
      shader.set_uniform_mat4("u_V", view);

      model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0, 1.0, -3.0));
      model = glm::scale(model, glm::vec3(1.0f));

      if (rotate) {
        model = glm::rotate(model, render.get_time() * glm::radians(40.0f), glm::vec3(0.5f, 5.0f, 1.0f));
      };

      shader.set_uniform_vec3("u_Light.position", light.position);
      shader.set_uniform_vec3("u_Light.ambient", light.ambient);
      shader.set_uniform_vec3("u_Light.diffuse", light.diffuse);
      shader.set_uniform_vec3("u_Light.specular", light.specular);

      shader.set_uniform_float1("u_Material.shininess", material.shininess);

      shader.set_uniform_mat4("u_M", model);
      shader.set_uniform_mat4("u_P", proj);

      render.draw(va, ib, shader); // todo: movo to a batch render and draw once;

      texture_wood.unbind();
      shader.unbind();
    }

    // third cube - the light
    {
      shader_light.bind();
      texture_light.bind();
      shader_light.set_uniform_mat4("u_V", view);

      shader_light.set_uniform_vec3("u_Light.ambient", light.ambient);
      shader_light.set_uniform_vec3("u_Light.diffuse", light.diffuse);

      light.position.x = 4.0f * sin(render.get_time());
      light.position.z = 5.0f * cos(render.get_time());

      model = glm::translate(glm::mat4(1.0f), light.position);
      model = glm::scale(model, glm::vec3(0.5));

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

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (first_mouse_call) {
    mouse_last_x = xpos;
    mouse_last_y = ypos;
    first_mouse_call = false;
  }

  float xoffset = xpos - mouse_last_x;
  float yoffset = mouse_last_y - ypos; // reversed since y-coordinates go from bottom to top

  mouse_last_x = xpos;
  mouse_last_y = ypos;

  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  if (state) {
    camera.process_mouse_moviment(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) { camera.process_mouse_scroll(yoffset); }
