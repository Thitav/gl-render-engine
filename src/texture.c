#define STB_IMAGE_IMPLEMENTATION

#include "texture.h"
#include "stb_image.h"

void texture_init(Texture *texture, GLenum type)
{
  texture->type = type;
  glGenTextures(1, &texture->handle);
}

void texture_load(Texture *texture, const char *path)
{
  stbi_set_flip_vertically_on_load(1);

  int width, height, channels;
  stbi_uc *data = stbi_load(path, &width, &height, &channels, 0);

  switch (channels)
  {
  case 4:
    glTexImage2D(texture->type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    break;
  default:
    glTexImage2D(texture->type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    break;
  }

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
