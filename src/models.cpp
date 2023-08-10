#include "models.hh"
#include "vendor/obj_loader/obj_loader.h"

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

  for (size_t m = 0; m < loader.LoadedMeshes.size(); m++) {
    for (size_t i = 0; i < loader.LoadedMeshes[m].Vertices.size(); i++) {
      Vertex v;
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
