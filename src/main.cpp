#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

static void
GL_debug_clear_error()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

static void
GL_debug_chek_error()
{
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
  }
}

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource
parse_source(const std::string& filepath)
{
  std::fstream stream(filepath);

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType shader_type = ShaderType::NONE;
  while (std::getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        shader_type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        shader_type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)shader_type] << line << "\n";
    }
  }

  return { ss[0].str(), ss[1].str() };
}

static unsigned int
compile_shader(unsigned int type_shader, const std::string& source)
{
  unsigned int id = glCreateShader(type_shader);
  const char* src = source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // error handling
  int error;
  glGetShaderiv(id, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* error_msg = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, error_msg);
    std::cout << "Fail shader compilation!" << std::endl;
    std::cout << error_msg << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int
create_sharder(const std::string& vertex_shader, const std::string& fragments_shader)
{
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

int
main(void)
{
  GLFWwindow* window;
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
  ShaderProgramSource source_code_shaders = parse_source("res/shaders/basic.shader");
  std::cout << "VEXTEX source code;" << std::endl;
  std::cout << source_code_shaders.VertexSource << std::endl;
  std::cout << "FRAGMENT source code:" << std::endl;
  std::cout << source_code_shaders.FragmentSource << std::endl;

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

  // bind VAO (vertex array)
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // buffer
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer); // setup vertex buffer
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(positions),
               positions,
               GL_STATIC_DRAW); // https://docs.gl/gl4/glBufferData difference between static and dynamic

  // Enable vertex buffer
  glEnableVertexAttribArray(0);

  // link this buffer to vao;
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * buffer_vertex_len, 0);

  // index buffer
  unsigned int ibuffer;
  glGenBuffers(1, &ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer); // setup indices buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  size_t indicies_len = sizeof(indices) / sizeof(unsigned int);

  // load shaders
  unsigned int shader = create_sharder(source_code_shaders.VertexSource, source_code_shaders.FragmentSource);
  glUseProgram(shader);

  // uniform
  int location = glGetUniformLocation(shader, "u_Color");
  glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);
  float r_color = 0.0f;
  float increment_color = 0.01f;

  // unbind vertex like this will disabled and make DrawElements without effect
  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw here */
    glUniform4f(location, r_color, 0.3f, 0.8f, 1.0f);

    // set/bind which buffer will use, the vao link to the actual vertex binded
    glUseProgram(shader);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);

    GL_debug_clear_error();
    glDrawElements(GL_TRIANGLES, indicies_len, GL_UNSIGNED_INT, nullptr);
    GL_debug_chek_error();

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

  glDeleteProgram(shader);
  glfwTerminate();
  return 0;
}
