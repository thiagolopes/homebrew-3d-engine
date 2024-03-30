#include "shaders.hh"

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

Shader::Shader(const std::string &filepath) : source_filepath(filepath), shader_id(0) {
    ShaderSourceCode source_code_shaders = parse_source(filepath);
    shader_id = create_sharder(source_code_shaders.VertexSource, source_code_shaders.FragmentSource);
};

Shader::~Shader() { glDeleteProgram(shader_id); };

void Shader::bind() const { glUseProgram(shader_id); };
void Shader::unbind() const { glUseProgram(0); };

void Shader::set_uniform_int1(const std::string &name, int value) { glUniform1i(get_uniform_location(name), value); };

void Shader::set_uniform_float1(const std::string &name, float v0) { glUniform1f(get_uniform_location(name), v0); };

void Shader::set_uniform_float3(const std::string &name, float v0, float v1, float v2) {
    glUniform3f(get_uniform_location(name), v0, v1, v2);
};

void Shader::set_uniform_float4(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
};

void Shader::set_uniform_vec3(const std::string &name, glm::vec3 &vec) {
    glUniform3fv(get_uniform_location(name), 1, &vec[0]);
};

void Shader::set_uniform_mat4(const std::string &name, glm::mat4 &matrix) {
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
};

int Shader::get_uniform_location(const std::string &name) {
    if (uniform_location_cache.find(name) != uniform_location_cache.end())
        return uniform_location_cache[name];

    int location = glGetUniformLocation(shader_id, name.c_str());
    if (location == -1)
        std::cout << "[WARNING] uniform '" << name << "[" << source_filepath << "] 'doenst not exist" << std::endl;

    uniform_location_cache[name] = location;
    return location;
};

ShaderSourceCode Shader::parse_source(const std::string &filepath) {
    std::fstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;
    std::stringstream ss[2];
    ShaderType shader_type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shader_type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos)
            { shader_type = ShaderType::FRAGMENT; }
        } else
        {
            ss[(int)shader_type] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
};

unsigned int Shader::compile_shader(unsigned int type_shader, const std::string &source) {
    unsigned int id = glCreateShader(type_shader);
    const GLchar *src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // error handling
    int error;
    glGetShaderiv(id, GL_COMPILE_STATUS, &error);
    if (error == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> error_msg(length);
        glGetShaderInfoLog(id, length, &length, error_msg.data());
        std::cout << "Fail shader [" << source_filepath << "] compilation! \n";
        std::cout << error_msg.data() << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
};

unsigned int Shader::create_sharder(const std::string &vertex_shader, const std::string &fragments_shader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragments_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // perform validation on shader
    glLinkProgram(program);
    glValidateProgram(program);

    // perform cleaning
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderUniform::ShaderUniform(Shader &shader) : _shader(shader){};

ShaderUniform::~ShaderUniform() { std::cout << "[DEBUG] ShaderUniform destroyed" << std::endl; }

void ShaderUniform::setup_uniforms(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, PointLight &point_light,
                                   DirLight *dir_light) {
    set_MVP(model, view, projection);
    set_point_light(point_light);
    if (dir_light != nullptr)
        set_directional_light(*dir_light);
}

void ShaderUniform::set_MVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
    glm::mat4 mvp = projection * view * model;
    _shader.set_uniform_mat4("u_MVP", mvp);
    _shader.set_uniform_mat4("u_ModelViewMatrix", model);
}

// TODO Move set_X_light to a ShaderUniform(Shader shader) class
void ShaderUniform::set_point_light(PointLight &pl) {
    _shader.set_uniform_vec3("u_PointLight.position", pl.position);
    _shader.set_uniform_vec3("u_PointLight.ambient", pl.ambient);
    _shader.set_uniform_vec3("u_PointLight.diffuse", pl.diffuse);
    _shader.set_uniform_vec3("u_PointLight.specular", pl.specular);
    _shader.set_uniform_float1("u_PointLight.constant", pl.constant);
    _shader.set_uniform_float1("u_PointLight.linear", pl.linear);
    _shader.set_uniform_float1("u_PointLight.quadratic", pl.quadratic);
}

void ShaderUniform::set_directional_light(DirLight &dl) {
    _shader.set_uniform_vec3("u_DirLight.direction", dl.direction);
    _shader.set_uniform_vec3("u_DirLight.ambient", dl.ambient);
    _shader.set_uniform_vec3("u_DirLight.diffuse", dl.diffuse);
    _shader.set_uniform_vec3("u_DirLight.specular", dl.specular);
}
