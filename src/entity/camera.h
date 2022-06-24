#include <cglm/cglm.h>
#ifndef CAMERA_H
#define CAMERA_H

const double CAM_SPEED;

typedef struct Camera {
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	mat4 lookAt_mat;
	mat4 perspective_mat;

	//orientation
	//quaterninion vec, direction[0] = w || 1-3 are: x, y, z
	vec3 direction;
	vec4 quaternion;
	float theta;
	float phi;
	float roll; //placeholder, typically 0
} Camera;

void Camera_init(Camera *camera);
void calc_orientation(Camera *camera);

//extern struct Camera camera;

#endif
