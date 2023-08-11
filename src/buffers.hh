#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <assert.h>

struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coord;
  glm::vec3 normal;
};

class VertexBuffer {
 private:
  unsigned int vb_render_id;

 public:
  VertexBuffer(const std::vector<Vertex> &data);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};

class IndexBuffer {
 private:
  unsigned int ib_render_id;
  unsigned int ib_count;

 public:
  IndexBuffer(const std::vector<unsigned int> data);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

  inline unsigned int get_count() const { return ib_count; }
};

struct VertexBufferElements {
  unsigned int type;
  unsigned int count;
  unsigned int normalize;

  static unsigned int get_size_of_type(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return 4;
      case GL_UNSIGNED_INT:
        return 4;
      case GL_UNSIGNED_BYTE:
        return 1;
    }
    std::cout << "[ERROR] GL type " << type << "not found" << std::endl;
    return 0;
  }
};

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElements> vbl_elements;
  unsigned int vbl_stride;

 public:
  VertexBufferLayout() : vbl_stride(0) {}

  template <typename T>
  void push(unsigned int count);


  inline const std::vector<VertexBufferElements> &get_elements() const { return vbl_elements; };
  inline unsigned int get_stride() const { return vbl_stride; };
};

class VertexArray {
 private:
  unsigned int va_render_id;

 public:
  VertexArray();
  ~VertexArray();

  void add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
  void bind() const;
  void unbind() const;
};
