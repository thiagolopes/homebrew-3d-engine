#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include "renderer.hh"
#include "buffers.hh"
#include "shaders.hh"

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

  // load shader source code
  Shader shader("res/shaders/basic.shader");
  shader.bind();
  shader.set_uniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  // draw steps:
  // 1. vertex buffer (in vram; collections of vertex)
  // 2. shadder (describe how to rasterization will render the collecttion of
  // vertex aka vertex buffer)

  // triagle possitions and incidices
  int buffer_vertex_len = 2;
  float positions[] = {
      -.5f, -.5f, // 0
      .5f,  -.5f, // 1
      .5f,  .5f,  // 2
      -.5f, .5f,  // 3
  };
  unsigned int indices[] = {
      0, 1, 2, // first triangle position
      2, 3, 0, // second ..
  };
  size_t indicies_len = sizeof(indices) / sizeof(unsigned int);

  // bind VAO (vertex array)
  VertexArray va;
  VertexBuffer vb(positions, sizeof(positions));
  VertexBufferLayout vbl;

  vbl.push<float>(buffer_vertex_len);
  va.add_buffer(vb, vbl);

  // index buffer
  IndexBuffer ib(indices, indicies_len);

  // uniform
  float r_color = 0.0f;
  float increment_color = 0.01f;

  // unbind vertex like this will disabled and make DrawElements without effect
  va.unbind();
  shader.unbind();
  vb.unbind();
  ib.unbind();

  Renderer render;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    render.clear();

    /* draw here */
    shader.bind();
    shader.set_uniform4f("u_Color", r_color, 0.3f, 0.8f, 1.0f);

    // render
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
