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
  size_t directory_len;
} Model;

void model_init(Model *model, const char *path);
void model_destroy(Model *model);
void model_draw(Model *model, Shader *shader);

#endif
