#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "buffers.hh"
#include "shaders.hh"
#include "imgui.h"
#include "models.hh"
#include "entity.hh"

void GL_debug_clear_error();
void GL_debug_chek_error();

class Renderer {
public:
    Renderer(bool z_buffer = true, bool frame_cap = true);
    void draw_model(const Model &model);
    void draw_mesh(Mesh &mesh);
    void draw_entity(const Entity &entity);
    void draw(const VertexArray &va, const IndexBuffer &ib) const;
    void draw_instancied(const VertexArray &va, const IndexBuffer &ib, const int many) const;
    void set_depth_test(bool flag = true) const;
    void set_swap_interval(bool flag = true) const;
};

class ImGuiRenderer {
public:
    ImGuiRenderer(GLFWwindow *window);
    ~ImGuiRenderer();

    void draw() const;
    void begin_frame() const;

    template <typename T> void debug(T &t);
};
