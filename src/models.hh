#pragma once

#include <vector>
#include "materials.hh"
#include "mesh.hh"

class Model {
 private:
  std::vector<Mesh *> meshes;
  Material *material;

 public:
  explicit Model(const std::string &model_name);
  ~Model();
  void draw(Renderer &render, Shader &shader);
};
