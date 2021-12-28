#include <cglm/cglm.h>
#ifndef CAMERA_H
#define CAMERA_H

const float CAM_SPEED;

struct Camera {
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	mat4 lookAt_mat;

	//orientation
	vec3 direction;
	float theta;
	float phi;
};

void Camera_init();
void calc_orientation();

extern struct Camera camera;

#endif
