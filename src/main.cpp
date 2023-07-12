#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int compile_shader(unsigned int type_shader,
                                   const std::string &source) {
  unsigned int id = glCreateShader(type_shader);
  const char *src = source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // error handling
  int error;
  glGetShaderiv(id, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *error_msg = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, error_msg);
    std::cout << "Fail shader compilation!" << std::endl;
    std::cout << error_msg << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int create_sharder(const std::string &vertex_shader,
                                   const std::string &fragments_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragments_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);

  // perform validation on shader
  glLinkProgram(program);
  glValidateProgram(program);

  // perform cleaning
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void) {
  GLFWwindow *window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // init glew
  if (glewInit() != GLEW_OK)
    std::cout << "Fail in load graphical drive" << std::endl;

  // show opengl version
  std::cout << glGetString(GL_VERSION) << std::endl;

  // 1. vertex buffer (in vram; collections of vertex)
  // 2. shadder (describe how to rasterization will render the collecttion of
  // vertex aka vertex buffer)

  // triagle possitions
  float positions_buffer[6] = {
      -.5f, -.5f, .0f, .5f, .5f, -.5f,
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer); // setup vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions_buffer), positions_buffer,
               GL_STATIC_DRAW); // https://docs.gl/gl4/glBufferData difference
                                // between static and dynamic

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  std::string vertex_shader = "#version 330 core\n"
                              "layout(location = 0) in vec4 position;"
                              "\n"
                              "void main(){\n"
                              "gl_Position = position;\n"
                              "}\n";
  std::string fragment_shader = "#version 330 core\n"
                                "layout(location = 0) out vec4 color;"
                                "\n"
                                "void main(){\n"
                                "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                "}\n";
  unsigned int shader = create_sharder(vertex_shader, fragment_shader);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw here */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);
  glfwTerminate();
  return 0;
}
