#ifndef SHADER_H
#define SHADER_H

#include <cglm/struct.h>

#include "glh.h"

typedef struct shader_t
{
  GLuint handle, vs_handle, fs_handle;
} Shader;

int shader_init(Shader *shader, char *vs_path, char *fs_path);
void shader_destroy(Shader *shader);
void shader_bind(Shader *shader);
void shader_uniform_int(Shader *shader, char *name, GLint i);
void shader_uniform_uint(Shader *shader, char *name, GLuint i);
void shader_uniform_float(Shader *shader, char *name, GLfloat f);
void shader_uniform_vec3(Shader *shader, char *name, vec3s v);
void shader_uniform_mat4(Shader *shader, char *name, mat4s m);

#endif
