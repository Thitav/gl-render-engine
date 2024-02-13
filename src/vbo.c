#include "vbo.h"

void vbo_init(VBO *vbo, GLenum type, GLboolean dynamic)
{
  vbo->type = type;
  vbo->dynamic = dynamic;
  glGenBuffers(1, &vbo->handle);
}

void vbo_destroy(VBO *vbo)
{
  glDeleteBuffers(1, &vbo->handle);
}

void vbo_bind(VBO *vbo)
{
  glBindBuffer(vbo->type, vbo->handle);
}

void vbo_buffer(VBO *vbo, void *data, GLsizeiptr offset, GLsizeiptr size)
{
  glBufferData(vbo->type, size - offset, data, vbo->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
