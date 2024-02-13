#ifndef MATERIAL_H
#define MATERIAL_H

#include <cglm/struct.h>

#include "glh.h"

typedef struct material_t
{
  GLint diffuse, specular, emission;
  GLfloat shininess;
} Material;

#endif
