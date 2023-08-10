#include "models.hh"
#include "vendor/obj_loader/obj_loader.h"

objl::Loader loader;

Model::Model(const std::string &model_name) {
  std::string path_models = "res/models/";
  std::string obj_name = model_name;
  std::string obj_path = path_models + obj_name;
  std::cout << "[DEBUG] " << obj_path << std::endl;

  if (!loader.LoadFile(obj_path + "/" + obj_name + ".obj")) {
    std::cout << "[ERROR] Fail to load model" << std::endl;
  }

  meshes.reserve(loader.LoadedMeshes.size());
  for (size_t m = 0; m < loader.LoadedMeshes.size(); m++) {
    std::vector<Vertex> vertices(loader.LoadedMeshes[m].Vertices.size());
    std::vector<unsigned int> indices(loader.LoadedMeshes[m].Indices.size());
    Vertex v;

    for (size_t i = 0; i < loader.LoadedMeshes[m].Vertices.size(); i++) {
      v.position.x = loader.LoadedMeshes[m].Vertices[i].Position.X;
      v.position.y = loader.LoadedMeshes[m].Vertices[i].Position.Y;
      v.position.z = loader.LoadedMeshes[m].Vertices[i].Position.Z;

      v.normal.x = loader.LoadedMeshes[m].Vertices[i].Normal.X;
      v.normal.y = loader.LoadedMeshes[m].Vertices[i].Normal.Y;
      v.normal.z = loader.LoadedMeshes[m].Vertices[i].Normal.Z;

      v.tex_coord.x = loader.LoadedMeshes[m].Vertices[i].TextureCoordinate.X;
      v.tex_coord.y = loader.LoadedMeshes[m].Vertices[i].TextureCoordinate.Y;
      vertices.push_back(v);
    }

    for (size_t i = 0; i < loader.LoadedIndices.size(); i++) {
      indices.push_back(loader.LoadedIndices[i]);
    }

    meshes.push_back(new Mesh(vertices, indices));
  }

  material =
      new Material(obj_path + "/" + loader.LoadedMaterials[0].map_Kd, obj_path + "/" + loader.LoadedMaterials[0].map_Ks,
                   obj_path + "/" + loader.LoadedMaterials[0].map_Ke, loader.LoadedMaterials[0].Ns, 1.0f, false);
}

Model::~Model() {
  for (size_t i = 0; i < meshes.size(); i++) {
    delete meshes[i];
  }
}

void Model::draw(Renderer &render, Shader &shader) {
  material->bind(shader);
  for (size_t i = 0; i < meshes.size(); i++) {
    meshes[i]->draw(render, shader);
  }
  material->unbind();
}
