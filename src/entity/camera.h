#include <cglm/cglm.h>
#ifndef CAMERA_H
#define CAMERA_H

const float CAM_SPEED;

struct Camera {
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
};

void Camera_init();

extern struct Camera camera;

#endif
