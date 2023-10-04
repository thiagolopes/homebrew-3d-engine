#pragma once

#include <glm/glm.hpp>
#include "imgui.h"
#include "vendor/imgui/imgui.h"

// types os lights:
struct PointLight
{
  glm::vec3 position;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  PointLight(float position, float ambient, float diffuse, float specular, float constant = 0.0f, float linear = 0.00f,
             float quadratic = 0.00f)
      : position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant),
        linear(linear){};

  inline void
  debug_menu()
  {
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

struct DirLight
{
  glm::vec3 direction;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  DirLight(float direction, float ambient, float diffuse, float specular)
      : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular){};

  inline void
  debug_menu()
  {
    ImGui::Begin("Direction Light");

    ImGui::SliderFloat3("Direction", &direction.x, -100, 100);
    ImGui::ColorEdit3("Ambiant light", &ambient.x);
    ImGui::ColorEdit3("Diffuse light", &diffuse.x);
    ImGui::ColorEdit3("Specular light", &specular.x);
    ImGui::End();
  }
};
