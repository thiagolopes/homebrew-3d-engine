#include "buffers.hh"
#include "renderer.hh"

VertexBuffer::VertexBuffer(const std::vector<Vertex> &data) {
  // setup vertex buffer
  // https://docs.gl/gl4/glBufferData difference between static and dynamic

  glGenBuffers(1, &vb_render_id);
  glBindBuffer(GL_ARRAY_BUFFER, vb_render_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void *)&data[0], GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &vb_render_id); }
void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, vb_render_id); }
void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

IndexBuffer::IndexBuffer(const std::vector<unsigned int> data) : ib_count(data.size()) {
  assert(sizeof(unsigned int) == sizeof(GLuint));

  glGenBuffers(1, &ib_render_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_render_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib_count * sizeof(unsigned int), (unsigned int *)&data[0], GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteFramebuffers(1, &ib_render_id); }

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_render_id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

VertexArray::VertexArray() { glGenVertexArrays(1, &va_render_id); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &va_render_id); }

void VertexArray::bind() const { glBindVertexArray(va_render_id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
  bind();
  vb.bind();

  const std::vector<VertexBufferElements> &elements = layout.get_elements();
  unsigned int offset = 0;
  for (size_t i = 0; i < elements.size(); i++) {
    const VertexBufferElements element = elements[i];

    // Enable vertex buffer
    glEnableVertexAttribArray(i);
    // link this buffer to vao;
    glVertexAttribPointer(i, element.count, element.type, element.normalize, layout.get_stride(),
                          reinterpret_cast<void *>(static_cast<uintptr_t>(offset)));
    // (void *)(uintptr_t)offset);
    offset += element.count * VertexBufferElements::get_size_of_type(element.type);
  }
}

template <> void VertexBufferLayout::push<float>(unsigned int count) {
  vbl_elements.push_back({GL_FLOAT, count, GL_FALSE});
  vbl_stride += count * VertexBufferElements::get_size_of_type(GL_FLOAT);
}

template <> void VertexBufferLayout::push<unsigned int>(unsigned int count) {
  vbl_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
  vbl_stride += count * VertexBufferElements::get_size_of_type(GL_UNSIGNED_INT);
}

template <> void VertexBufferLayout::push<unsigned char>(unsigned int count) {
  vbl_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
  vbl_stride += count * VertexBufferElements::get_size_of_type(GL_UNSIGNED_BYTE);
}
