#pragma once

#include <glm/glm.hpp>
#include "materials.hh"
#include "mesh.hh"

class Entity {
 private:
  Mesh *_mesh;
  Material *_material;

  glm::vec3 _position;
  glm::mat4 _word_position;

  glm::vec3 _scale;
  float _angle;

 public:
  Entity(Mesh *mesh, Material *material, glm::vec3 position, float scale = 1.0f, float angle = 0);
  void inc_position(float x, float y, float z);
  void inc_scale(float scale);
  void inc_angle(float angle);
  glm::mat4 &word_position();
};
