#include "materials.hh"

Material::Material(const std::string &diffuse_path, const std::string &specular_path, const std::string &emission_path,
                   float shininess, float emissioness, bool emission_mask)
    : m_diffuse(diffuse_path), m_specular(specular_path), m_emission(emission_path), m_shininess(shininess),
      m_emissioness(emissioness), m_emission_mask(emission_mask){};

void
Material::bind(Shader &shader)
{
  m_diffuse.bind(0);
  m_specular.bind(1);
  m_emission.bind(2);

  shader.set_uniform_int1("u_Material.diffuse", 0);
  shader.set_uniform_int1("u_Material.specular", 1);
  shader.set_uniform_int1("u_Material.emission", 2);

  shader.set_uniform_float1("u_Material.shininess", m_shininess);
  shader.set_uniform_float1("u_Material.emission_level", m_emissioness);
  shader.set_uniform_int1("u_Material.emission_mask", m_emission_mask);
}

void
Material::unbind()
{
  m_diffuse.unbind();
  m_specular.unbind();
  m_emission.unbind();
};
