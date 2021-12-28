#include "camera.h"

const float CAM_SPEED = 0.10f;

struct Camera camera;

void Camera_init() {
	glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera.cameraPos);
	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.cameraFront);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
	glm_mat4_copy(GLM_MAT4_ZERO, camera.lookAt_mat);
	camera.theta = 270.0f;
	camera.phi = 0.0f;
}

void calc_orientation() {
	//theta init to 270, bc -z is facing forward, phi is 0
	//x and z are influenced by pitch bc since if we look up, the x/z will shrink in accordance to unit circle
	//obv the direction won't change but it's length will
	//if didn't adjust it would be a square like box look.
	glm_mat4_copy(GLM_MAT4_ZERO, camera.lookAt_mat);
	camera.direction[0] = cos(glm_rad(camera.theta)) * cos(glm_rad(camera.phi));
	camera.direction[1] = sin(glm_rad(camera.phi));
	camera.direction[2] = sin(glm_rad(camera.theta)) * cos(glm_rad(camera.phi));
	glm_normalize(camera.direction);
	glm_vec3_copy(camera.direction, camera.cameraFront);
	//glm_vec3_add(camera.cameraPos, camera.cameraFront, camera.cameraFront);
	//tmp vec lets us look straight
	//glm_lookat(camera.cameraPos, camera.cameraFront, camera.cameraUp, camera.lookAt_mat);
	//printf("%f\n", camera.theta);
}
