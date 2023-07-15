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
    // positiion    //texture possition
    -50.0f, -50.0f, 0.0f, 0.0f, 0.0f, // 0
    50.0f,  -50.0f, 0.0f, 1.0f, 0.0f, // 1
    50.0f,  50.0f,  0.0f, 1.0f, 1.0f, // 2
    -50.0f, 50.0f,  0.0f, 0.0f, 1.0f  // 3
};

unsigned int indices[] = {
    0, 1, 3, // first triangle position
    1, 2, 3,

};
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
  float scaleA = 1;
  float scaleB = 1;

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
      ImGui::SliderFloat3("Translation A", &translationA.x, -1000.0f, 1000.0f);
      ImGui::SliderFloat("Scale A", &scaleA, 0.0f, 10.0f);
      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ImGui::SliderFloat("Angle B", &angleB, 0.0f, TAU);
      ImGui::SliderFloat3("RotationB", &rotateB.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Translation B", &translationB.x, -1000.0f, 1000.0f);
      ImGui::SliderFloat("Scale B", &scaleB, 0.0f, 10.0f);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui_io.Framerate, imgui_io.Framerate);
      ImGui::End();
    }

    /* draw here */
    shader.bind();

    {
      // first texture
      model = glm::translate(glm::mat4(1.0f),
                             translationA * glm::vec3(1.0, 1.0, -1.0)); // multiply z to use positive bar in gui
      model = glm::rotate(model, angleA, rotateA);
      model = glm::scale(model, glm::vec3(scaleA, scaleA, scaleA));

      mvp = proj * view * model;
      shader.set_uniform_mat4("u_MVP", mvp);

      render.draw(va, ib, shader);
    }

    {
      // second textures
      model = glm::translate(glm::mat4(1.0f), translationB * glm::vec3(1.0, 1.0, -1.0));
      model = glm::rotate(model, angleB, rotateB);
      model = glm::scale(model, glm::vec3(scaleB, scaleB, scaleB));

      mvp = proj * view * model;
      shader.set_uniform_mat4("u_MVP", mvp);

      render.draw(va, ib, shader);
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
