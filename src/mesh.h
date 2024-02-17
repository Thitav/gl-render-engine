#ifndef MESH_H
#define MESH_H

#include <cglm/struct.h>

#include "glh.h"
#include "texture.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"

typedef struct vertex_t
{
  vec3s position, normal;
  vec2s tex_coords;
} Vertex;

typedef struct texture_group_t
{
  Texture *diffusion, *specular, *emission;
  GLuint ndiffusion, nspecular, nemission;
} TextureGroup;

typedef struct mesh_t
{
  Vertex *vertices;
  GLuint *indices;
  TextureGroup texture_group;
  GLuint nvertices, nindices;
  VBO vbo, ebo;
  VAO vao;
} Mesh;

void mesh_init(Mesh *mesh, Vertex *vertices, GLuint nvertices, GLuint *indices, GLuint nindices, TextureGroup texture_group);
void mesh_destroy(Mesh *mesh);
void mesh_draw(Mesh *mesh, Shader *shader);

#endif
