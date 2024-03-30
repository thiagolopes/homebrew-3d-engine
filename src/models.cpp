#include "models.hh"
#include "vendor/obj_loader/obj_loader.h"
#include <memory>

// CAUTION: This implemention blend all the meshes of file in one;
Model::Model(const std::string &dir_name) : name(dir_name) {
    objl::Loader loader;
    std::string obj_path = "res/models/" + name + "/";
    if (!loader.LoadFile(obj_path + name + ".obj"))
    {
        std::cout << "[ERROR] Fail to load model: " << obj_path + name + ".obj" << std::endl;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
    {
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

    for (size_t i = 0; i < loader.LoadedIndices.size(); i++)
    {
        indices.push_back(loader.LoadedIndices[i]);
    }

    mesh.reset(new Mesh(vertices, indices));

    std::string diffuse_path = obj_path + loader.LoadedMaterials[0].map_Kd;
    std::string specular_path = obj_path + loader.LoadedMaterials[0].map_Ks;
    std::string emission_path = obj_path + loader.LoadedMaterials[0].map_Ke;
    float specular_expoent = loader.LoadedMaterials[0].Ns;

    material.reset(new Material(diffuse_path, specular_path, emission_path, specular_expoent, 1.0, false));
}

Model::~Model() {
    mesh.reset();
    material.reset();
    std::cout << "[DEBUG] Model " << name << " destroyed" << std::endl;
}
