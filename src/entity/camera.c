#include "camera.h"

const float CAM_SPEED = 2.0f;

struct Camera camera;

void Camera_init() {
	glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera.cameraPos);
	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.cameraFront);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
}
