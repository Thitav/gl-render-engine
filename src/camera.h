#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/struct.h>

#include "glh.h"

typedef struct perspective_camera_t
{
  mat4s view, projection;
  vec3s position, direction, up, right;
  float pitch, yaw, fov, aspect, znear, zfar;
} PerspectiveCamera;

void camera_perspective_init(PerspectiveCamera *camera, float fov, float aspect, float znear, float zfar);
void camera_perspective_update(PerspectiveCamera *camera);
void camera_perspective_keyboard(PerspectiveCamera *camera, GLenum key, float delta_time);
void camera_perspective_mouse(PerspectiveCamera *camera, float xoffset, float yoffset);
void camera_perspective_scroll(PerspectiveCamera *camera, float yoffset);

#endif
