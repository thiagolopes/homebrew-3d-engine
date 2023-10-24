#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "lights.hh"

struct ShaderSourceCode
{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader
{
  // a Shader class does:
  // - load source code and compile a shader
  // - bind
  // - unbind
  // - set uniforms to a shader
private:
  std::string source_filepath;
  unsigned int shader_id;
  std::unordered_map<std::string, int> uniform_location_cache;
  // caching for uniforms

  unsigned int compile_shader(unsigned int type_shader, const std::string &source);

public:
  explicit Shader(const std::string &filepath);
  ~Shader();

  void bind() const;
  void unbind() const;
  ShaderSourceCode parse_source(const std::string &filepath);
  unsigned int create_sharder(const std::string &vertex_shader, const std::string &fragments_shader);

  void set_uniform_int1(const std::string &name, int value);
  void set_uniform_float1(const std::string &name, float v0);
  void set_uniform_float3(const std::string &name, float v0, float v1, float v2);
  void set_uniform_float4(const std::string &name, float v0, float v1, float v2, float v3);
  void set_uniform_mat4(const std::string &name, glm::mat4 &matrix);
  void set_uniform_vec3(const std::string &name, glm::vec3 &vec);

  void set_MVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
  void set_point_light(PointLight &point_light);
  void set_directional_light(DirLight &dir_light);

  // TODO update to auto load from the source code and set automatic the uniforms
  int get_uniform_location(const std::string &name);
};

class ShaderUniform
{
private:
  Shader& _shader;
  void set_MVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
  void set_point_light(PointLight &point_light);
  void set_directional_light(DirLight &dir_light);

public:
  ShaderUniform(Shader& shader);
  ~ShaderUniform();

  void setup_uniforms(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection,
                      PointLight &point_light, DirLight *dir_light);
};
