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
};

struct DirLight
{
  glm::vec3 direction;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  DirLight(float direction, float ambient, float diffuse, float specular)
      : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular){};
};
