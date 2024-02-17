#ifndef TEXTURE_H
#define TEXTURE_H

#include "glh.h"

typedef struct texture_t
{
  GLuint handle;
  GLenum type;
} Texture;

void texture_init(Texture *texture, GLenum type);
void texture_load(Texture *texture, const char *path);
void texture_bind(Texture *texture, GLuint slot);
void texture_param(Texture *texture, GLenum param, GLenum value);

#endif
