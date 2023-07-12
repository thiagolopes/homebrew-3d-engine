#include "buffers.hh"
#include "renderer.hh"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
  glGenBuffers(1, &vb_render_id);
  glBindBuffer(GL_ARRAY_BUFFER, vb_render_id); // setup vertex buffer
  glBufferData(GL_ARRAY_BUFFER,
               size,
               data,
               GL_STATIC_DRAW); // https://docs.gl/gl4/glBufferData difference between static and dynamic
}
VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &vb_render_id);
}
void
VertexBuffer::bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, vb_render_id);
}
void
VertexBuffer::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
  : ib_count(count)
{
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  glGenBuffers(1, &ib_render_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_render_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
  glDeleteFramebuffers(1, &ib_render_id);
}

void
IndexBuffer::bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_render_id);
}

void
IndexBuffer::unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
