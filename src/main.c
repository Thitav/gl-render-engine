#include <stdio.h>
#include <cglm/struct.h>
#include <string.h>

#include "glh.h"
#include "vao.h"
#include "vbo.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "mesh.h"
#include "model.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Window resize callback function
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

PerspectiveCamera camera;
float lastx = 400, lasty = 300;
int first_mouse = 1;
float delta_time, last_frame = 0.0f;

// Process input function
void process_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_perspective_keyboard(&camera, GLFW_KEY_W, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_perspective_keyboard(&camera, GLFW_KEY_S, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_perspective_keyboard(&camera, GLFW_KEY_A, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_perspective_keyboard(&camera, GLFW_KEY_D, delta_time);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (first_mouse)
  {
    lastx = (float)xpos;
    lasty = (float)ypos;
    first_mouse = 0;
  }

  float xoffset = (float)xpos - lastx;
  float yoffset = lasty - (float)ypos;
  lastx = (float)xpos;
  lasty = (float)ypos;

  camera_perspective_mouse(&camera, xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera_perspective_scroll(&camera, (float)yoffset);
}

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
// unsigned int indices[] = {
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test", NULL, NULL);
  if (window == NULL)
  {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  Shader shader;
  if (!shader_init(&shader, "src/shaders/shader.vs", "src/shaders/shader.fs"))
  {
    printf("Failed to create shader program\n");
    return -1;
  }

  Shader light_shader;
  if (!shader_init(&light_shader, "src/shaders/light.vs", "src/shaders/light.fs"))
  {
    printf("Failed to create shader program\n");
    return -1;
  }

  VBO vbo;
  vbo_init(&vbo, GL_ARRAY_BUFFER, GL_FALSE);
  vbo_bind(&vbo);
  vbo_buffer(&vbo, vertices, 0, sizeof(vertices));

  VAO vao_cube, vao_light;
  vao_init(&vao_cube);
  vao_init(&vao_light);

  vao_bind(&vao_cube);
  vbo_bind(&vbo);
  vao_attrib(0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  vao_attrib(1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
  vao_attrib(2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));

  vao_bind(&vao_light);
  vbo_bind(&vbo);
  vao_attrib(0, 3, GL_FLOAT, 8 * sizeof(float), 0);

  // VBO ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_FALSE);
  // vbo_bind(ebo);
  // vbo_buffer(ebo, indices, 0, sizeof(indices));

  Texture container_diff;
  texture_init(&container_diff, GL_TEXTURE_2D);
  texture_bind(&container_diff, 0);
  texture_load(&container_diff, "resources/container_diff.png");

  Texture container_spec;
  texture_init(&container_spec, GL_TEXTURE_2D);
  texture_bind(&container_spec, 1);
  texture_load(&container_spec, "resources/container_spec.png");

  Texture container_emis;
  texture_init(&container_emis, GL_TEXTURE_2D);
  texture_bind(&container_emis, 2);
  texture_load(&container_emis, "resources/matrix.jpg");

  // texture_param(texture_container, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // texture_param(texture_container, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // texture_param(texture_container, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // texture_param(texture_container, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Material cube_mat = {.diffuse = 0,
                       .specular = 1,
                       .emission = 3,
                       .shininess = 32.0f};
  Light light_props = {.ambient = (vec3s){0.2f, 0.2f, 0.2f},
                       .diffuse = (vec3s){0.5f, 0.5f, 0.5f},
                       .specular = (vec3s){1.0f, 1.0f, 1.0f},
                       .position = (vec3s){3.0f, 1.0f, 2.0f}};

  shader_bind(&shader);
  shader_uniform_int(&shader, "material.diffuse", cube_mat.diffuse);
  shader_uniform_int(&shader, "material.specular", cube_mat.specular);
  shader_uniform_int(&shader, "material.emission", cube_mat.emission);
  shader_uniform_float(&shader, "material.shininess", cube_mat.shininess);
  shader_uniform_vec3(&shader, "light.ambient", light_props.ambient);
  shader_uniform_vec3(&shader, "light.diffuse", light_props.diffuse);
  shader_uniform_vec3(&shader, "light.specular", light_props.specular);
  shader_uniform_vec3(&shader, "light.position", light_props.position);

  camera_perspective_init(&camera, glm_rad(45), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);

  // Model backpack_model;
  // model_init(&backpack_model, "resources/backpack/backpack.obj");

  Model test_model;
  model_init(&test_model, "resources/cube.obj");

  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window))
  {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_perspective_update(&camera);
    mat4s model = glms_mat4_identity();

    shader_bind(&shader);
    shader_uniform_mat4(&shader, "model", model);
    shader_uniform_mat4(&shader, "view", camera.view);
    shader_uniform_mat4(&shader, "projection", camera.projection);
    shader_uniform_vec3(&shader, "viewPos", camera.position);

    model_draw(&test_model, &shader);

    // vao_bind(&vao_cube);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // model_draw(&model, &shader);

    model = glms_mat4_identity();
    model = glms_translate(model, light_props.position);
    model = glms_scale(model, (vec3s){0.2f, 0.2f, 0.2f});

    shader_bind(&light_shader);
    shader_uniform_mat4(&light_shader, "model", model);
    shader_uniform_mat4(&light_shader, "view", camera.view);
    shader_uniform_mat4(&light_shader, "projection", camera.projection);

    vao_bind(&vao_light);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // model_destroy(&backpack_model);
  model_destroy(&test_model);
  vao_destroy(&vao_cube);
  vao_destroy(&vao_light);
  vbo_destroy(&vbo);
  // vbo_destroy(ebo);
  shader_destroy(&shader);
  glfwTerminate();

  return 0;
}
