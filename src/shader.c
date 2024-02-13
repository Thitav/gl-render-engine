#include <stdlib.h>
#include <stdio.h>

#include "shader.h"

char *_shader_load(const char *path)
{
  FILE *fptr = fopen(path, "rb");

  fseek(fptr, 0, SEEK_END);
  size_t fsize = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char *buff = malloc(fsize + 1);
  fread(buff, fsize, sizeof(char), fptr);
  buff[fsize] = 0;

  fclose(fptr);
  return buff;
}

int _shader_compile(const GLchar *source, GLenum type, GLuint *shader_out)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char info_log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
    printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
  }

  *shader_out = shader;
  free(source);
  return success;
}

int _program_link(GLuint vs, GLuint fs, GLuint *program_out)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int success;
  char info_log[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(program, sizeof(info_log), NULL, info_log);
    printf("ERROR::SHADER_PROGRAM::LINKING_FAILED\n%s\n", info_log);
  }

  *program_out = program;
  return success;
}

int shader_init(Shader *shader, char *vs_path, char *fs_path)
{
  char *vs_source = _shader_load(vs_path);
  char *fs_source = _shader_load(fs_path);

  GLuint vs, fs;
  if (!_shader_compile(vs_source, GL_VERTEX_SHADER, &vs))
  {
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    return 0;
  }
  if (!_shader_compile(fs_source, GL_FRAGMENT_SHADER, &fs))
  {
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    return 0;
  }

  GLuint shader_program;
  if (!_program_link(vs, fs, &shader_program))
  {
    printf("ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED\n");
    return 0;
  }

  shader->handle = shader_program;
  shader->vs_handle = vs;
  shader->fs_handle = fs;
  return 1;
}

void shader_destroy(Shader *shader)
{
  glDeleteProgram(shader->handle);
  glDeleteShader(shader->vs_handle);
  glDeleteShader(shader->fs_handle);
}

void shader_bind(Shader *shader)
{
  glUseProgram(shader->handle);
}

void shader_uniform_int(Shader *shader, char *name, GLint i)
{
  glUniform1i(glGetUniformLocation(shader->handle, name), i);
}

void shader_uniform_uint(Shader *shader, char *name, GLuint i)
{
  glUniform1ui(glGetUniformLocation(shader->handle, name), i);
}

void shader_uniform_float(Shader *shader, char *name, GLfloat f)
{
  glUniform1f(glGetUniformLocation(shader->handle, name), f);
}

void shader_uniform_vec3(Shader *shader, char *name, vec3s v)
{
  glUniform3fv(glGetUniformLocation(shader->handle, name), 1, (GLfloat *)&v.raw);
}

void shader_uniform_mat4(Shader *shader, char *name, mat4s m)
{
  glUniformMatrix4fv(glGetUniformLocation(shader->handle, name), 1, GL_FALSE, (GLfloat *)&m.raw);
}
