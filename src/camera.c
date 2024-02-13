#include "camera.h"

void camera_perspective_init(PerspectiveCamera *camera, float fov, float aspect, float znear, float zfar)
{
  camera->fov = fov;
  camera->aspect = aspect;
  camera->znear = znear;
  camera->zfar = zfar;
}

void camera_perspective_update(PerspectiveCamera *camera)
{
  camera->direction = glms_normalize((vec3s){cosf(glm_rad(camera->pitch)) * cosf(glm_rad(camera->yaw)),
                                             sinf(glm_rad(camera->pitch)),
                                             cosf(glm_rad(camera->pitch)) * sinf(glm_rad(camera->yaw))});
  camera->right = glms_normalize(glms_cross(camera->direction, (vec3s){0.0f, 1.0f, 0.0f}));
  camera->up = glms_normalize(glms_cross(camera->right, camera->direction));

  camera->view = glms_lookat(camera->position, glms_vec3_add(camera->position, camera->direction), camera->up);
  camera->projection = glms_perspective(glm_rad(camera->fov), camera->aspect, camera->znear, camera->zfar);
}

void camera_perspective_keyboard(PerspectiveCamera *camera, GLenum key, float delta_time)
{
  float velocity = 3 * delta_time;
  if (key == GLFW_KEY_W)
    camera->position = glms_vec3_add(camera->position, glms_vec3_scale(camera->direction, velocity));
  if (key == GLFW_KEY_S)
    camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->direction, velocity));
  if (key == GLFW_KEY_A)
    camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->right, velocity));
  if (key == GLFW_KEY_D)
    camera->position = glms_vec3_add(camera->position, glms_vec3_scale(camera->right, velocity));
}

void camera_perspective_mouse(PerspectiveCamera *camera, float xoffset, float yoffset)
{
  xoffset = xoffset * 0.2f;
  yoffset = yoffset * 0.2f;

  camera->yaw += xoffset;
  camera->pitch += yoffset;

  // camera->pitch = glm_clamp(camera->pitch, -GLM_2_PI, GLM_2_PI);
  // camera->yaw = (camera->yaw < 0 ? GLM_PI * 2 : 0.0f) + fmodf(camera->yaw, GLM_PI * 2);

  if (camera->pitch > 89.0f)
    camera->pitch = 89.0f;
  if (camera->pitch < -89.0f)
    camera->pitch = -89.0f;
}

void camera_perspective_scroll(PerspectiveCamera *camera, float yoffset)
{
  camera->fov -= yoffset;
  if (camera->fov < 1.0f)
    camera->fov = 1.0f;
  if (camera->fov > 45.0f)
    camera->fov = 45.0f;
}
