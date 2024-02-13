#ifndef VAO_H
#define VAO_H

#include "glh.h"

typedef struct vao_t
{
  GLuint handle;
} VAO;

void vao_init(VAO *vao);
void vao_destroy(VAO *vao);
void vao_bind(VAO *vao);
void vao_attrib(GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);

#endif
