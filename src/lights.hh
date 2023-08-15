#pragma once

#include <glm/glm.hpp>
#include "shaders.hh"
#include "imgui.h"
#include "vendor/imgui/imgui.h"

// types os lights:
struct PointLight {
  glm::vec3 position;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  void set_on_shader(Shader &shader) {
    shader.bind();

    shader.set_uniform_vec3("u_PointLight.position", position);

    shader.set_uniform_vec3("u_PointLight.ambient", ambient);
    shader.set_uniform_vec3("u_PointLight.diffuse", diffuse);
    shader.set_uniform_vec3("u_PointLight.specular", specular);

    shader.set_uniform_float1("u_PointLight.constant", constant);
    shader.set_uniform_float1("u_PointLight.linear", linear);
    shader.set_uniform_float1("u_PointLight.quadratic", quadratic);
  }

  inline void debug_menu() {
    ImGui::Begin("Point Light");
    ImGui::SliderFloat3("Position", &position.x, -100, 100);

    ImGui::ColorEdit3("Ambiant light", &ambient.x);
    ImGui::ColorEdit3("Diffuse light", &diffuse.x);
    ImGui::ColorEdit3("Specular light", &specular.x);

    ImGui::SliderFloat("Constant", &constant, 0.0, 1.0, "%.4f");
    ImGui::SliderFloat("Linear", &linear, 0.0, 1.0, "%.4f");
    ImGui::SliderFloat("Quadratic", &quadratic, 0.0, 1.0, "%.4f");
    ImGui::End();
  }
};

struct DirLight {
  glm::vec3 direction;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  void set_on_shader(Shader &shader) {
    shader.bind();

    shader.set_uniform_vec3("u_DirLight.direction", direction);

    shader.set_uniform_vec3("u_DirLight.ambient", ambient);
    shader.set_uniform_vec3("u_DirLight.diffuse", diffuse);
    shader.set_uniform_vec3("u_DirLight.specular", specular);
  }

  inline void debug_menu() {
    ImGui::Begin("Direction Light");

    ImGui::SliderFloat3("Direction", &direction.x, -100, 100);
    ImGui::ColorEdit3("Ambiant light", &ambient.x);
    ImGui::ColorEdit3("Diffuse light", &diffuse.x);
    ImGui::ColorEdit3("Specular light", &specular.x);
    ImGui::End();
  }
};
