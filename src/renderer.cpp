#include "renderer.hh"
#include <cstddef>
#include <iostream>
#include <GLFW/glfw3.h>

void
GL_debug_clear_error()
{
  while (glGetError() != GL_NO_ERROR)
    {
    };
};

void
GL_debug_chek_error()
{
  while (GLenum error = glGetError())
    {
      std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
};

Renderer::Renderer(bool z_buffer, bool frame_cap)
{
  set_depth_test(z_buffer);
  set_swap_interval(frame_cap);
}

void
Renderer::draw(const VertexArray &va, const IndexBuffer &ib) const
{
  va.bind();
  ib.bind();

  GL_debug_clear_error();
  glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
  GL_debug_chek_error();
};

void
Renderer::set_swap_interval(bool flag) const
{
  glfwSwapInterval(flag);
};

void
Renderer::set_depth_test(bool flag) const
{
  if (flag)
    {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
    }
  else
    glDisable(GL_DEPTH_TEST);
};

ImGuiRenderer::ImGuiRenderer(GLFWwindow *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
};

ImGuiRenderer::~ImGuiRenderer()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void
ImGuiRenderer::draw() const
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
};

void
ImGuiRenderer::begin_frame() const
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
