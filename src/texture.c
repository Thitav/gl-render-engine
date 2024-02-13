#define STB_IMAGE_IMPLEMENTATION

#include "texture.h"
#include "stb_image.h"

void texture_init(Texture *texture, GLenum type)
{
  texture->type = type;
  glGenTextures(1, &texture->handle);
}

void texture_load(Texture *texture, const char *path, GLenum format, GLenum source_format)
{
  stbi_set_flip_vertically_on_load(1);

  int width, height, channels;
  stbi_uc *data = stbi_load(path, &width, &height, &channels, 0);

  glTexImage2D(texture->type, 0, format, width, height, 0, source_format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(texture->type);

  stbi_image_free(data);
}

void texture_bind(Texture *texture, GLuint slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(texture->type, texture->handle);
}

void texture_param(Texture *texture, GLenum param, GLenum value)
{
  glTexParameteri(texture->type, param, value);
}
