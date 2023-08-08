#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

#include "shaders.hh"
#include "mesh.hh"
#include "materials.hh"
#include "renderer.hh"

class Model {
 private:
  std::vector<Mesh> m_meshes;
  std::string m_path;
  Material m_material;

  void assimp_load(std::string path);
  void process_node(aiNode *node, const aiScene *scene);
  Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
  Material Process_material(aiMaterial *mat, aiTextureType type, std::string typeName);

 public:
  Model(char *path);
  void draw(Renderer &render, Shader &shader);
};
