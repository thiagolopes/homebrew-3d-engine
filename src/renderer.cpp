#include "renderer.hh"
#include <cstddef>
#include <iostream>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

void GL_debug_clear_error() {
  while (glGetError() != GL_NO_ERROR) {
  };
};

void GL_debug_chek_error() {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
  }
};

Renderer::Renderer(char *window_name, float width, float height)
    : m_window(nullptr), m_width(width), m_height(height), m_deltatime(0), m_lastframe(0) {
  /* Initialize the library */
  if (!glfwInit()) {
    std::cerr << "[ERROR] In load GLFW!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // set opengl to 3 and use CORE. this will make VAO especification mandatory
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  m_window = glfwCreateWindow(m_width, m_height, window_name, nullptr, nullptr);
  if (!m_window) {
    std::cerr << "[ERROR] Fail in create window GLFW!" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_window);
  // init glew
  if (glewInit() != GLEW_OK) {
    std::cout << "[ERROR] Fail in load graphical drive" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // show opengl version
  std::cout << "[LOG]" << glGetString(GL_VERSION) << std::endl;
  update_deltatime_frame();
};

Renderer::~Renderer() { glfwTerminate(); };

void Renderer::set_mouse_scroll_callback(void *f) { glfwSetScrollCallback(get_window(), (GLFWscrollfun)f); };

void Renderer::set_mouse_button_callback(void *f) { glfwSetMouseButtonCallback(get_window(), (GLFWmousebuttonfun)f); };

void Renderer::set_mouse_moviment_callback(void *f) { glfwSetCursorPosCallback(get_window(), (GLFWcursorposfun)f); };

float Renderer::get_time() { return glfwGetTime(); }

void Renderer::end_frame() {
  update_deltatime_frame();
  glfwSwapBuffers(m_window);
  glfwPollEvents();
};

void Renderer::set_swap_interval(bool flag) const { glfwSwapInterval(flag); };

void Renderer::set_depth_test(bool flag) const {
  if (flag)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
};

bool Renderer::running() { return !glfwWindowShouldClose(m_window); };

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
  shader.bind();
  va.bind();
  ib.bind();

  GL_debug_clear_error();
  glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
  GL_debug_chek_error();
};

void Renderer::clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };

ImGuiRenderer::ImGuiRenderer(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
};

ImGuiRenderer::~ImGuiRenderer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void ImGuiRenderer::draw() const {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
};

void ImGuiRenderer::clear() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
