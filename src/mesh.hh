#pragma once

#include "buffers.hh"

class Mesh {
private:
  VertexBuffer m_vb;
  IndexBuffer m_ib;
  VertexArray m_va;
  VertexBufferLayout m_vbl;

public:
  Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indices);

  inline VertexArray& vertex_array() {
    return m_va;
  };
  inline IndexBuffer& index_buffer() {
    return m_ib;
  };
};
