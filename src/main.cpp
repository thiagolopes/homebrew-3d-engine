#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"
#include "textures.hh"

// triagle positions and incidices - vertex collection

float positions[] = {
    // positiion      texture possition
    100.0f, 100.0f, 0.0f, 0.0f, // 0
    200.0f, 100.0f, 1.0f, 0.0f, // 1
    200.0f, 200.0f, 1.0f, 1.0f, // 2
    100.0f, 200.0f, 0.0f, 1.0f  // 3
};

unsigned int indices[] = {
    0, 1, 2, // first triangle position
    2, 3, 0, // second ..
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // set fps cap
  glfwSwapInterval(1);

  // init glew
  if (glewInit() != GLEW_OK)
    std::cout << "Fail in load graphical drive" << std::endl;

  // show opengl version
  std::cout << glGetString(GL_VERSION) << std::endl;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);    // 4:3
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); // move "camera" to left
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(500, 0, 0)); // move "camera" to top

  glm::mat4 mvp = proj * view * model;

  // draw steps:
  // 1. vertex buffer (in vram; collections of vertex)
  // 2. shadder (describe how to rasterization will render the collecttion of
  // vertex aka vertex buffer)

  // bind VAO (vertex array)
  VertexArray va;
  VertexBuffer vb(positions, sizeof(positions));
  IndexBuffer ib(indices, indicies_len);

  VertexBufferLayout vbl;
  vbl.push<float>(2);
  vbl.push<float>(2);

  va.add_buffer(vb, vbl);

  // load shader source code
  Shader shader("res/shaders/basic.shader");
  shader.bind();
  shader.set_uniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  shader.set_uniform_mat4("u_MVP", mvp);

  // texture
  Texture texture("res/textures/texture.jpg");
  texture.bind();
  shader.set_uniform1i("u_Texture", 0);

  // uniform
  float r_color = 0.0f;
  float increment_color = 0.01f;

  Renderer render;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    render.clear();

    /* draw here */
    shader.set_uniform4f("u_Color", r_color, 0.3f, 0.8f, 1.0f);
    render.draw(va, ib, shader);

    // r color change
    if (r_color > 1.0f)
      increment_color = increment_color * -1;
    else if (r_color < .0f)
      increment_color = abs(increment_color);
    r_color += increment_color;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
