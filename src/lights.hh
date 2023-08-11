#pragma once

#include <glm/glm.hpp>
#include "shaders.hh"

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
};
