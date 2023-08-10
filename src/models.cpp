#include "models.hh"
#include "vendor/obj_loader/obj_loader.h"

// CAUTION: This implemention blend all the meshes of file in one;
Model::Model(const std::string &model_name) {
  objl::Loader loader;
  std::string path_models = "res/models/";
  std::string obj_name = model_name;
  std::string obj_path = path_models + obj_name;

  if (!loader.LoadFile(obj_path + "/" + obj_name + ".obj")) {
    std::cout << "[ERROR] Fail to load model: " << obj_name << std::endl;
  }

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (size_t i = 0; i < loader.LoadedVertices.size(); i++) {
    Vertex v;
    v.position.x = loader.LoadedVertices[i].Position.X;
    v.position.y = loader.LoadedVertices[i].Position.Y;
    v.position.z = loader.LoadedVertices[i].Position.Z;

    v.normal.x = loader.LoadedVertices[i].Normal.X;
    v.normal.y = loader.LoadedVertices[i].Normal.Y;
    v.normal.z = loader.LoadedVertices[i].Normal.Z;

    v.tex_coord.x = loader.LoadedVertices[i].TextureCoordinate.X;
    v.tex_coord.y = loader.LoadedVertices[i].TextureCoordinate.Y;

    vertices.push_back(v);
  }
  for (size_t i = 0; i < loader.LoadedIndices.size(); i++) {
    indices.push_back(loader.LoadedIndices[i]);
  }
  mesh = (new Mesh(vertices, indices));

  material =
      new Material(obj_path + "/" + loader.LoadedMaterials[0].map_Kd, obj_path + "/" + loader.LoadedMaterials[0].map_Ks,
                   obj_path + "/" + loader.LoadedMaterials[0].map_Ke, loader.LoadedMaterials[0].Ns, 1.0f, false);
}

Model::~Model() {
  delete mesh;
  delete material;
}

void Model::draw(Renderer &render, Shader &shader) {
  material->bind(shader);
  mesh->draw(render, shader);
  material->unbind();
}
