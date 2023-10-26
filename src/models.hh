#pragma once

#include <memory>
#include <vector>
#include "materials.hh"
#include "mesh.hh"
#include "memory"

class Model
{
public:
  std::string name;
  std::unique_ptr<Mesh> mesh;
  std::unique_ptr<Material> material;

  explicit Model(const std::string &model_name);
  ~Model();
  void draw(Renderer &render, Shader &shader);

  Mesh* get_mesh() const {return mesh.get();};
  Material* get_material() const { return material.get();};
};
