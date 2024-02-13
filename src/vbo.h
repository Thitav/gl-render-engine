#ifndef VBO_H
#define VBO_H

#include "glh.h"

typedef struct vbo_t
{
  GLuint handle;
  GLenum type;
  GLboolean dynamic;
} VBO;

void vbo_init(VBO *vbo, GLenum type, GLboolean dynamic);
void vbo_destroy(VBO *vbo);
void vbo_bind(VBO *vbo);
void vbo_buffer(VBO *vbo, void *data, GLsizeiptr offset, GLsizeiptr size);

#endif
