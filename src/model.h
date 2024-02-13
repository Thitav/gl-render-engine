#ifndef MODEL_H
#define MODEL_H

#include <cglm/struct.h>

#include "glh.h"
#include "mesh.h"

typedef struct model_t
{
  Mesh *meshes;
  unsigned int nmeshes;
  char *directory;
} Model;

void model_load(const char *path);

#endif
