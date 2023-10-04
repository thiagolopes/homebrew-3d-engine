#include <vector>
#include <assert.h>

#include "mesh.hh"

Mesh::Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indices) : m_vb(vertex), m_ib(indices)
{
  int layout_ordered[] = { vertex[0].position.length(), vertex[0].tex_coord.length(), vertex[0].normal.length() };

  m_vbl.push<float>(layout_ordered[0]);
  m_vbl.push<float>(layout_ordered[1]);
  m_vbl.push<float>(layout_ordered[2]);

  m_va.add_buffer(m_vb, m_vbl);
};

void
Mesh::draw(Renderer &render, Shader &shader)
{
  shader.bind();
  render.draw(vertex_array(), index_buffer());
  shader.unbind();
};
