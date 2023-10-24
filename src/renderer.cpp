#include "renderer.hh"
#include "lights.hh"
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

/* In most cases, you'll want to call true to enable sync,
   but if you have a reason to disable it (for example,
   if you're comparing shader performance) you can should use false; */
void
Renderer::set_swap_interval(bool flag) const
{
  glfwSwapInterval(flag);
};

/* Enable to avoid unecessary vertex draw using Z-Buffer check,
   but you can disable and change the DepthFunc due face tests reason*/
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

template <>
void
ImGuiRenderer::debug(PointLight &pl)
{
  ImGui::Begin("Point Light");
  ImGui::SliderFloat3("Position", &pl.position.x, -100, 100);

  ImGui::ColorEdit3("Ambiant light", &pl.ambient.x);
  ImGui::ColorEdit3("Diffuse light", &pl.diffuse.x);
  ImGui::ColorEdit3("Specular light", &pl.specular.x);

  ImGui::SliderFloat("Constant", &pl.constant, 0.0, 1.0, "%.4f");
  ImGui::SliderFloat("Linear", &pl.linear, 0.0, 1.0, "%.4f");
  ImGui::SliderFloat("Quadratic", &pl.quadratic, 0.0, 1.0, "%.4f");
  ImGui::End();
};

template <>
void
ImGuiRenderer::debug(DirLight &dl)
{
  ImGui::Begin("Direction Light");
  ImGui::SliderFloat3("Direction", &dl.direction.x, -100, 100);
  ImGui::ColorEdit3("Ambiant light", &dl.ambient.x);

  ImGui::ColorEdit3("Diffuse light", &dl.diffuse.x);
  ImGui::ColorEdit3("Specular light", &dl.specular.x);
  ImGui::End();
};
