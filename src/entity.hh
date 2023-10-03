#pragma once

#include <glm/glm.hpp>
#include "materials.hh"
#include "mesh.hh"
#include "models.hh"

class Entity {
 private:
  Mesh *_mesh;
  Material *_material;

  glm::vec3 _position;
  glm::mat4 _word_position;

  glm::vec3 _scale;
  float _angle_degrees;

 public:
  Entity(Mesh *mesh, Material *material, glm::vec3 position, float scale = 1.0f, float angle_degrees = 0);
  Entity(Model &model, glm::vec3 position, float scale = 1.0f, float angle_degrees = 0);

  void position(float x, float y, float z);
  void inc_position(float x, float y, float z);

  void angle(float angle);
  void inc_angle(float angle);

  void scale(float scale);
  void scale(float scale_x, float scale_y, float scale_z);
  void inc_scale(float scale);

  glm::mat4 &word_position();
};
