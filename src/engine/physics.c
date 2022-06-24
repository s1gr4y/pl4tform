#include "physics.h"

const double GRAVITY = -0.2f/66.0f;
const double AIR_RESISTANCE = -0.01f/66.0f;	//-0.1f/66.0f;
const double GROUND_RESISTANCE = -0.2f/66.0f;	//-0.1f/66.0f;

void ComputePositionObj(Object obj) {
	exit(-1);
}

void ComputePositionPlayer(Player *player, double dt) {

	if (player->in_air == false) {
		ApplyGroundResistance(player->velFoward, dt);
		ApplyGroundResistance(player->velRight, dt);
		ApplyGroundResistance(player->velLeft, dt);
		ApplyGroundResistance(player->velBack, dt);
	} else {
		ApplyAirResistance(player->velFoward, dt);
		ApplyAirResistance(player->velRight, dt);
		ApplyAirResistance(player->velLeft, dt);
		ApplyAirResistance(player->velBack, dt);
	}
	float magnitudeFront = glm_vec3_norm(player->velFoward);
	float magnitudeSideR = glm_vec3_norm(player->velRight);
	float magnitudeBack = glm_vec3_norm(player->velBack);
	float magnitudeSideL = glm_vec3_norm(player->velLeft);
	//printf("dt: %f\n", dt);
	float maxFoward = CAM_SPEED * 2.5;
	float maxSide = CAM_SPEED * 2.25;
	float magTot = sqrtf(maxFoward + maxSide);
	//printf("maxFoward: %f\n", maxFoward);	//projection is same for forward+back / left+right
	//printf("magnitudeFront: %f\n", magnitudeFront);
	//printf("magnitudeBack: %f\n", magnitudeBack);
	//printf("magnitudeSideL: %f\n", magnitudeSideL);
	//printf("magnitudeSideR: %f\n", magnitudeSideR);
	if (magnitudeFront >= maxFoward) {
		magnitudeFront = maxFoward;
	}
	if (magnitudeBack >= maxSide) {
		magnitudeBack = maxSide;
	}
	if (magnitudeSideL >= maxSide) {
		magnitudeSideL = maxSide;
	}
	if (magnitudeSideR >= maxSide) {
		magnitudeSideR = maxSide;
	}

	if (magnitudeBack >= maxSide && magnitudeFront >= maxSide) {
		magnitudeFront = maxSide;
	}
	glm_normalize(player->velFoward);
	glm_normalize(player->velRight);
	glm_normalize(player->velLeft);
	glm_normalize(player->velBack);

	glm_vec3_scale(player->velFoward, magnitudeFront, player->velFoward);
	glm_vec3_scale(player->velBack, magnitudeBack, player->velBack);
	glm_vec3_scale(player->velLeft, magnitudeSideL, player->velLeft);
	glm_vec3_scale(player->velRight, magnitudeSideR, player->velRight);

	vec3 movementSum = GLM_VEC3_ZERO_INIT;
	glm_vec3_add(movementSum, player->velFoward, movementSum);
	glm_vec3_add(movementSum, player->velBack, movementSum);
	glm_vec3_add(movementSum, player->velLeft, movementSum);
	glm_vec3_add(movementSum, player->velRight, movementSum);


	vec3 rightVecProj = GLM_VEC3_ZERO_INIT;
	vec3 fowardVecProj = GLM_VEC3_ZERO_INIT;
	vec3 outRightProj = GLM_VEC3_ZERO_INIT;
	vec3 outFowardProj = GLM_VEC3_ZERO_INIT;
	rightVecProj[0] = movementSum[0]*cos(glm_rad(90.0f)) - movementSum[2]*sin(glm_rad(90.0f));
	rightVecProj[2] = movementSum[0]*sin(glm_rad(90.0f)) + movementSum[2]*cos(glm_rad(90.0f));
	glm_vec3_copy(movementSum, fowardVecProj);
	fowardVecProj[1] = 0.0f;
	///*
	if (glm_vec3_norm(fowardVecProj) >= 1e-8) {
		glm_vec3_proj(movementSum, rightVecProj, outRightProj);
		glm_vec3_proj(movementSum, fowardVecProj, outFowardProj);
		float frontProjSum = glm_vec3_norm(outFowardProj);
		float rightProjSum = glm_vec3_norm(outRightProj);
		glm_normalize(outRightProj);
		glm_normalize(outFowardProj);
		if (rightProjSum >= maxSide) {
			glm_vec3_scale(outRightProj, maxSide, outRightProj);
		} else {
			glm_vec3_scale(outRightProj, rightProjSum, outRightProj);
		}
		if (frontProjSum >= maxFoward) {
			glm_vec3_scale(outFowardProj, maxFoward, outFowardProj);
		} else {
			glm_vec3_scale(outFowardProj, frontProjSum, outFowardProj);
		}
		vec3 sumfinal = GLM_VEC3_ZERO_INIT;

		glm_vec3_add(outFowardProj, outRightProj, sumfinal);
		//printf("sumfinal post normal = %f, %f, %f\n", sumfinal[0], sumfinal[1], sumfinal[2]);
		glm_vec3_copy(sumfinal, movementSum);
	}
	//*/

	if (magTot >= glm_vec3_norm(movementSum)) {
		glm_vec3_copy(movementSum, player->velocity);
	} else {
		if (player->in_air == false) {
			ApplyGroundResistance(player->velocity, dt);
		} else {
			ApplyAirResistance(player->velocity, dt);
		}
	}

	if (player->velUp[1] < 0 && fabs(player->velUp[1]) >= fabs(GRAVITY * 100)) {
		//printf("%f >= %f\n",player->velocity[1], GRAVITY * 100);
		player->velUp[1] = GRAVITY * 100;
	} else {
		player->velUp[1] += dt * GRAVITY;
	}
	glm_vec3_add(player->velocity, player->velUp, player->velocity);
	//break the original velocity vector into front and right vector components, makes changes, then use unit vector from both and add together with scaling.


	glm_vec3_copy(player->velocity, player->prev_velocity);
	//printf("vel adding? %f %f %f\n", player->velocity[0], dt * player->velocity[1], dt * player->velocity[2]);
	player->coords[0] += dt * player->velocity[0];
	player->coords[1] += dt * player->velocity[1];
	player->coords[2] += dt * player->velocity[2];
	if (player->coords[1] <= -1.50f) {
		player->coords[1] = -1.50f;
		player->velocity[1] = 0.0f;
		player->velUp[1] = 0.0f;
		player->in_air = false;
	}
	//printf("vel player post-edt: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	//printf("player->velFoward: %f %f %f\n", player->velFoward[0], player->velFoward[1], player->velFoward[2]);
	//printf("player->velRight: %f %f %f\n", player->velRight[0], player->velRight[1], player->velRight[2]);
	//printf("player->velLeft: %f %f %f\n", player->velLeft[0], player->velLeft[1], player->velLeft[2]);
	//printf("player->velBack: %f %f %f\n", player->velBack[0], player->velBack[1], player->velBack[2]);
	glm_vec3_copy(player->coords, player->camera.cameraPos);
	player->camera.cameraPos[1] = player->camera.cameraPos[1] + 0.5f;
}

void ApplyGroundResistance(vec3 vector, double dt) {
	vec3 tmp = GLM_VEC3_ZERO_INIT;
	glm_vec3_copy(vector, tmp);
	glm_normalize(tmp);
	glm_vec3_negate(tmp);
	glm_vec3_scale(tmp, dt * -GROUND_RESISTANCE, tmp);
	if (fabs(tmp[0]) > fabs(vector[0])) {
		vector[0] = 0.0f;
	} else {
		vector[0] += tmp[0];	//sign1 * dt * AIR_GROUND_RESISTANCE;
	}
	if (fabs(tmp[2]) > fabs(vector[2])) {
		vector[2] = 0.0f;
	} else {
		vector[2] += tmp[2];	//sign3 * dt * AIR_GROUND_RESISTANCE;
	}
}

void ApplyAirResistance(vec3 vector, double dt) {
	vec3 tmp = GLM_VEC3_ZERO_INIT;
	glm_vec3_copy(vector, tmp);
	glm_normalize(tmp);
	glm_vec3_negate(tmp);
	glm_vec3_scale(tmp, dt * -AIR_RESISTANCE, tmp);
	if (fabs(tmp[0]) > fabs(vector[0])) {
		vector[0] = 0.0f;
	} else {
		vector[0] += tmp[0];	//sign1 * dt * AIR_GROUND_RESISTANCE;
	}
	if (fabs(tmp[2]) > fabs(vector[2])) {
		vector[2] = 0.0f;
	} else {
		vector[2] += tmp[2];	//sign3 * dt * AIR_GROUND_RESISTANCE;
	}
}

