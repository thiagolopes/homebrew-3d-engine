#pragma once

class VertexBuffer
{
private:
  unsigned int vb_render_id;

public:
  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};

class IndexBuffer
{
private:
  unsigned int ib_render_id;
  unsigned int ib_count;

public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

  inline unsigned int get_count() const
  {
    return ib_count;
  }
};
