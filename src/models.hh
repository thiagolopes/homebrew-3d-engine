#pragma once

#include <vector>
#include "materials.hh"
#include "mesh.hh"

class Model
{
public:
  Mesh *mesh;
  Material *material;

  explicit Model(const std::string &model_name);
  ~Model();
  void draw(Renderer &render, Shader &shader);
};
