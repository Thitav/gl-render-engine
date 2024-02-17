#include "mesh.h"

void mesh_init(Mesh *mesh, Vertex *vertices, GLuint nvertices, GLuint *indices, GLuint nindices, TextureGroup texture_group)
{
  mesh->vertices = vertices;
  mesh->indices = indices;
  mesh->nvertices = nvertices;
  mesh->nindices = nindices;
  mesh->texture_group = texture_group;

  vbo_init(&mesh->vbo, GL_ARRAY_BUFFER, GL_FALSE);
  vbo_init(&mesh->ebo, GL_ELEMENT_ARRAY_BUFFER, GL_FALSE);
  vao_init(&mesh->vao);

  vao_bind(&mesh->vao);

  vbo_bind(&mesh->vbo);
  vbo_buffer(&mesh->vbo, vertices, 0, nvertices * sizeof(Vertex));
  vbo_bind(&mesh->ebo);
  vbo_buffer(&mesh->ebo, indices, 0, nindices * sizeof(GLuint));

  vao_attrib(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
  vao_attrib(1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
  vao_attrib(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tex_coords));
}

void mesh_destroy(Mesh *mesh)
{
  free(mesh->vertices);
  free(mesh->indices);
  free(mesh->texture_group.diffusion);
  free(mesh->texture_group.specular);
  free(mesh->texture_group.emission);
  vbo_destroy(&mesh->vbo);
  vbo_destroy(&mesh->ebo);
  vao_destroy(&mesh->vao);
}

void mesh_draw(Mesh *mesh, Shader *shader)
{
  // TODO: multiple textures
  texture_bind(&mesh->texture_group.diffusion[0], 0);
  shader_uniform_int(shader, "material.diffuse", 0);
  texture_bind(&mesh->texture_group.specular[0], 1);
  shader_uniform_int(shader, "material.specular", 1);
  texture_bind(&mesh->texture_group.emission[0], 2);
  shader_uniform_int(shader, "material.emission", 2);

  vao_bind(&mesh->vao);
  glDrawElements(GL_TRIANGLES, mesh->nindices, GL_UNSIGNED_INT, 0);
  // vao_bind(0);
}
