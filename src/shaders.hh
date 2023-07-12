#include <string>

struct ShaderSourceCode {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader {
  // a Shader class does:
  // - load source code and compile a shader
  // - bind
  // - unbind
  // - set uniforms to a shader
private:
  std::string source_filepath;
  unsigned int shader_id;
  // caching for uniforms

  unsigned int compile_shader(unsigned int type_shader, const std::string &source);

public:
  Shader(const std::string &filepath);
  ~Shader();

  void bind() const;
  void unbind() const;

  ShaderSourceCode parse_source(const std::string &filepath);
  unsigned int create_sharder(const std::string &vertex_shader, const std::string &fragments_shader);
  // TODO update to auto load from the source code and set automatic the uniforms
  void set_uniform4f(const std::string &name, float v0, float v1, float v2, float v3);
  unsigned int get_uniform_location(const std::string &name);
};
