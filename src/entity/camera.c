#include "camera.h"

const double CAM_SPEED = 1.0f/66.0f;

//struct Camera camera;

void Camera_init(Camera *camera) {
	glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, camera->cameraPos);
	glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, camera->cameraFront);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->cameraUp);
	glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, camera->direction);
	glm_mat4_copy(GLM_MAT4_ZERO, camera->lookAt_mat);
	//glm_perspective(glm_rad(90.0f), dimWid / dimHigh, 0.1f, 100.0f, camera->perspective_mat);
	camera->theta = 0.0f;
	camera->phi = 0.0f;
	camera->roll = 0.0f;
}

void calc_orientation(Camera *camera) {
	glm_mat4_copy(GLM_MAT4_ZERO, camera->lookAt_mat);
	///*
	camera->direction[0] = cos(glm_rad(camera->theta)) * cos(glm_rad(camera->phi));
	camera->direction[1] = sin(glm_rad(camera->phi));
	camera->direction[2] = sin(glm_rad(camera->theta)) * cos(glm_rad(camera->phi));
	glm_normalize(camera->direction);
	glm_vec3_copy(camera->direction, camera->cameraFront);
	//*/
	//consider quarternion
	/*
	double cy = cos(glm_rad(camera.theta * 0.5));
    double sy = sin(glm_rad(camera.theta * 0.5));
    double cp = cos(glm_rad(camera.phi * 0.5));
    double sp = sin(glm_rad(camera.phi * 0.5));
    double cr = cos(glm_rad(camera.roll * 0.5));
    double sr = sin(glm_rad(camera.roll * 0.5));

   // Quaternion q;
    camera.quaternion[3] = cr * cp * cy + sr * sp * sy;
    camera.quaternion[0] = sr * cp * cy - cr * sp * sy;
    camera.quaternion[1] = cr * sp * cy + sr * cp * sy;
    camera.quaternion[2] = cr * cp * sy - sr * sp * cy;
    glm_normalize(camera.quaternion);
	*/
	vec3 tempVec;
	glm_vec3_zero(tempVec);
	glm_vec3_copy(camera->direction, camera->cameraFront);
	glm_vec3_add(camera->cameraPos, camera->cameraFront, tempVec);

	///*
	if (camera->phi <= -270.0f) {
		glm_vec3_copy((vec3){-cos(glm_rad(camera->theta)), 0.0f, -sin(glm_rad(camera->theta))}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else if (camera->phi <= -180.0f) {
		glm_vec3_copy((vec3){0.0f, -1.0f, 0.0f}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else if (camera->phi <= -90.0f) { //camera.phi <= -90.0f
		glm_vec3_copy((vec3){cos(glm_rad(camera->theta)), 0.0f, sin(glm_rad(camera->theta))}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else if (camera->phi >= 270.0f) {
		glm_vec3_copy((vec3){cos(glm_rad(camera->theta)), 0.0f, sin(glm_rad(camera->theta))}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else if (camera->phi >= 180.0f) {
		glm_vec3_copy((vec3){0.0f, -1.0f, 0.0f}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else if (camera->phi >= 90.0f) {
		glm_vec3_copy((vec3){-cos(glm_rad(camera->theta)), 0.0f, -sin(glm_rad(camera->theta))}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	} else { //between 90 and -90
		//set upvector to be the based on player theta with x and z ignore y
		glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->cameraUp);
		glm_lookat(camera->cameraPos, tempVec, camera->cameraUp, camera->lookAt_mat);
	}

	/*
	if (camera.phi <= -90.0f) { //camera.phi <= -90.0f
		glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
		glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
	} else {
		//set upvector to be the based on player theta with x and z ignore y
		glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
		glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
	}
	*/
}

