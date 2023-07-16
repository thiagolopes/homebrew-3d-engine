#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

#include "vendor/imgui/imgui.h"

#define TAU 6.28

int m_width = 960;
int m_height = 540;

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
  GLFWwindow *window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  // set opengl to 3 and use CORE. this will make VAO especification mandatory
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(m_width, m_height, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // set fps cap
  glfwSwapInterval(false);

  // start depth test
  glEnable(GL_DEPTH_TEST);

  // init glew
  if (glewInit() != GLEW_OK)
    std::cout << "Fail in load graphical drive" << std::endl;

  // show opengl version
  std::cout << glGetString(GL_VERSION) << std::endl;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // draw steps:
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
  Texture texture("res/textures/texture.jpg");
  texture.bind();
  shader.set_uniform1i("u_Texture", 0);

  Renderer render;

  // imgui
  ImGuiRenderer imgui(window);
  ImGuiIO &imgui_io = ImGui::GetIO();
  (void)imgui_io;

  // So, basically MVP:
  // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
  // View matrix: defines position and orientation of the "camera".
  // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
  glm::mat4 model;
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
  // glm::mat4 proj = glm::ortho(0.0f, (float)m_width, 0.0f, float(m_height), -1.0f, 1.0f);
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_height / (float)m_width, 0.1f, 1000.0f);

  glm::mat4 mvp;

  glm::vec3 rotateA(1.0f, 1.0f, 1.0f);
  glm::vec3 rotateB(1.0f, 1.0f, 1.0f);

  glm::vec3 translationA(0, 0, 217.0f);
  glm::vec3 translationB(0, 110, 500.0f);
  float angleA = 0;
  float angleB = 0;
  float scaleA = 30;
  float scaleB = 50;

  bool rotate = true;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    imgui.clear();
    render.clear();

    // set mvp in imgui
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
    shader.bind();

    {
      // first cube

      model = glm::translate(glm::mat4(1.0f),
                             translationA * glm::vec3(1.0, 1.0, -1.0)); // multiply z to use positive bar in gui
      model = glm::rotate(model, angleA, rotateA);
      model = glm::scale(model, glm::vec3(scaleA, scaleA, scaleA));

      if (rotate) {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      };

      mvp = proj * view * model;
      shader.set_uniform_mat4("u_MVP", mvp);

      render.draw(va, ib, shader);
    }

    {
      // second cube

      model = glm::translate(glm::mat4(1.0f), translationB * glm::vec3(1.0, 1.0, -1.0));
      model = glm::rotate(model, angleB, rotateB);
      model = glm::scale(model, glm::vec3(scaleB, scaleB, scaleB));

      if (rotate) {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      };

      mvp = proj * view * model;
      shader.set_uniform_mat4("u_MVP", mvp);

      render.draw(va, ib, shader); // todo: movo to a batch render and draw once;
    }

    imgui.draw();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  // shutdown
  glfwTerminate();
  return 0;
}
