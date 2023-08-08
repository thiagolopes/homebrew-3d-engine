#pragma once

#include "textures.hh"
#include "shaders.hh"

class Material {
 private:
  Texture m_diffuse;
  Texture m_specular;
  Texture m_emission;
  float m_shininess;
  float m_emissioness;
  bool m_emission_mask;

 public:
  Material(const std::string& diffuse_path, const std::string& specular_path, const std::string& emission_path,
           float shininess, float emissioness, bool emission_mask);
  void bind(Shader& shader);
  void unbind();
};
