#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"

Texture *_load_material_textures(Model *model, struct aiMaterial *material, enum aiTextureType type, unsigned int *ntextures_out)
{
  unsigned int ntextures = aiGetMaterialTextureCount(material, type);
  Texture *textures = malloc(sizeof(Texture) * ntextures);

  for (unsigned int i = 0; i < ntextures; i++)
  {
    struct aiString filename;
    aiGetMaterialTexture(material, type, i, &filename, 0, 0, 0, 0, 0, 0);

    char *path = calloc(model->directory_len + filename.length + 1, sizeof(char));
    strcat(path, model->directory);
    strcat(path, filename.data);

    texture_init(&textures[i], GL_TEXTURE_2D);
    texture_load(&textures[i], path);

    free(path);
  }

  *ntextures_out = ntextures;
  return textures;
}

void _process_mesh(Model *model, const struct aiScene *scene, struct aiMesh *mesh, unsigned int mesh_index)
{
  // Vertices
  Vertex *vertices = malloc(sizeof(Vertex) * mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    vertices[i].position = (vec3s){mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
    vertices[i].normal = (vec3s){mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
    if (mesh->mTextureCoords[0])
    {
      vertices[i].tex_coords = (vec2s){mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
    }
    else
    {
      vertices[i].tex_coords = (vec2s){0.0f, 0.0f};
    }
  }

  // Indices
  unsigned int nindices = 0;
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    nindices += mesh->mFaces[i].mNumIndices;
  }
  GLuint *indices = malloc(sizeof(GLuint) * nindices);
  unsigned int cindices = 0;
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    struct aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices[cindices] = face.mIndices[j];
      cindices++;
    }
  }

  // Material
  TextureGroup texture_group;
  if (mesh->mMaterialIndex >= 0)
  {
    struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    unsigned int ntextures;

    texture_group.diffusion = _load_material_textures(model, material, aiTextureType_DIFFUSE, &ntextures);
    texture_group.ndiffusion = ntextures;

    texture_group.specular = _load_material_textures(model, material, aiTextureType_SPECULAR, &ntextures);
    texture_group.nspecular = ntextures;

    texture_group.emission = _load_material_textures(model, material, aiTextureType_EMISSIVE, &ntextures);
    texture_group.nemission = ntextures;
  }

  mesh_init(&model->meshes[mesh_index], vertices, mesh->mNumVertices, indices, nindices, texture_group);
}

void _process_node(Model *model, const struct aiScene *scene, struct aiNode *node)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    _process_mesh(model, scene, mesh, node->mMeshes[i]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    _process_node(model, scene, node->mChildren[i]);
  }
}

void model_init(Model *model, const char *path)
{
  const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
    return;
  }

  size_t directory_len = (strrchr(path, '/') - path) + sizeof(char);
  model->directory = malloc(directory_len + sizeof(char));
  memcpy(model->directory, path, directory_len);
  model->directory[directory_len] = '\0';
  model->directory_len = directory_len;

  model->meshes = malloc(sizeof(Mesh) * scene->mNumMeshes);
  model->nmeshes = scene->mNumMeshes;

  _process_node(model, scene, scene->mRootNode);
  aiReleaseImport(scene);
}

void model_destroy(Model *model)
{
  for (unsigned int i = 0; i < model->nmeshes; i++)
  {
    mesh_destroy(&model->meshes[i]);
  }
  free(model->meshes);
  free(model->directory);
}

void model_draw(Model *model, Shader *shader)
{
  for (unsigned int i = 0; i < model->nmeshes; i++)
  {
    mesh_draw(&model->meshes[i], shader);
  }
}
