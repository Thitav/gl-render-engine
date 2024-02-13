#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"

void _load_material_textures(struct aiMaterial *material, enum aiTextureType ai_type)
{
  for (unsigned int i = 0; i < aiGetMaterialTextureCount(material, ai_type); i++)
  {
    struct aiString path;
    aiGetMaterialTexture(material, ai_type, i, &path, 0, 0, 0, 0, 0, 0);
    Texture texture;
    texture_init(&texture, GL_TEXTURE_2D);
    // WIP
    // texture_load(&texture, path, );
  }
}

void _process_mesh(const struct aiScene *scene, struct aiMesh *mesh)
{
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

  unsigned int nindices = 0;
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    nindices += mesh->mFaces[i].mNumIndices;
  }
  GLuint *indices = malloc(sizeof(GLuint) * nindices);
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    struct aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices[i + j] = face.mIndices[j];
    }
  }

  if (mesh->mMaterialIndex >= 0)
  {
    struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  }
}

void _process_node(Model *model, const struct aiScene *scene, struct aiNode *node)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    // model->meshes[node->mMeshes[i]] = _process_mesh;
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

  model->meshes = malloc(sizeof(Mesh) * scene->mNumMeshes);
  model->nmeshes = scene->mNumMeshes;
}

void model_destroy(Model *model)
{
  free(model->meshes);
}
