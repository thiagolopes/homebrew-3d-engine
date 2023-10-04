#pragma once

#include "buffers.hh"
#include "renderer.hh"
#include "shaders.hh"

class Mesh
{
private:
  VertexBuffer m_vb;
  IndexBuffer m_ib;
  VertexArray m_va;
  VertexBufferLayout m_vbl;

public:
  Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indices);

  void draw(Renderer &render, Shader &shader);
  inline VertexArray &
  vertex_array()
  {
    return m_va;
  };
  inline IndexBuffer &
  index_buffer()
  {
    return m_ib;
  };
};
