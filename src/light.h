#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/struct.h>

typedef struct light_t
{
  vec3s position;
  vec3s ambient;
  vec3s diffuse;
  vec3s specular;
} Light;

#endif
