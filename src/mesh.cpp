#include <vector>
#include <assert.h>

#include "mesh.hh"

Mesh::Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indices) : m_vb(vertex), m_ib(indices) {
  m_vbl.push<float>(vertex[0].position.length());
  m_vbl.push<float>(vertex[0].tex_coord.length());
  m_vbl.push<float>(vertex[0].normal.length());

  m_va.add_buffer(m_vb, m_vbl);
};

void Mesh::draw(Renderer& render, Shader& shader) { render.draw(m_va, m_ib, shader); };
