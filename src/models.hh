#pragma once

#include <vector>
#include "materials.hh"
#include "mesh.hh"

class Model {
 private:
  Mesh *mesh;
  Material *material;

 public:
  explicit Model(const std::string &model_name);
  ~Model();
  void draw(Renderer &render, Shader &shader);
};
