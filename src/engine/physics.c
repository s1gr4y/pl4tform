#include "physics.h"

const double GRAVITY = -0.3f/66.0f;
const double AIR_RESISTANCE = -0.01f/66.0f;	//-0.1f/66.0f;
const double GROUND_RESISTANCE = -0.15f/66.0f;	//-0.1f/66.0f;

void ComputePositionObj(Object obj) {
	exit(-1);
}

void ComputePositionPlayer(Player *player, double dt) {
	//phys mostly done, just need to fix how player gets velocity based on angle relative to obj/obj's velocity
	printf("vel player start-edt: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	if (player->in_air == false) {
		ApplyGroundResistance(player->velUp, dt);
		ApplyGroundResistance(player->velMoveNormal, dt);
		ApplyGroundResistance(player->velocity, dt);
	} else {
		ApplyAirResistance(player->velUp, dt);
		ApplyAirResistance(player->velMoveNormal, dt);
		ApplyAirResistance(player->velocity, dt);
	}


	//glm_vec3_add(player->velMove, player->velMoveNormal, player->velMoveNormal);
	float maxSpeed = CAM_SPEED * 2.5;

	vec3 movementSum = GLM_VEC3_ZERO_INIT;
	if (player->in_air == true) {
		vec3 tmp = GLM_VEC3_ZERO_INIT;
		glm_vec3_add(player->velAdded, player->velMoveNormal, tmp);
		glm_vec3_copy(tmp, player->velocity);
		/*
		glm_vec3_add(player->velAdded, player->velocity, player->velocity);
		glm_vec3_add(player->velocity, player->velMoveNormal, tmp);
		if (glm_vec3_norm(player->velocity) > glm_vec3_norm(tmp) || glm_vec3_norm(player->velocity) <= maxSpeed) {	//need diff method
			glm_vec3_copy(tmp, player->velocity);
		}
		*/
	} else {
		vec3 tmp = GLM_VEC3_ZERO_INIT;
		printf("velAdded: %f, %f, %f\n", player->velAdded[0], player->velAdded[1], player->velAdded[2]);
		printf("velMoveNormal: %f, %f, %f\n", player->velMoveNormal[0], player->velMoveNormal[1], player->velMoveNormal[2]);
		glm_vec3_add(player->velAdded, player->velMoveNormal, tmp);
		glm_vec3_copy(tmp, player->velocity);
	}

	if (player->velUp[1] < 0.0f && fabs(player->velUp[1]) >= fabs(GRAVITY * 40)) {
		player->velUp[1] = GRAVITY * 40;
	} else {
		player->velUp[1] += dt * GRAVITY;
	}
	printf("vel up is %f\n", player->velUp[1]);
	//player->velocity[1] += player->velUp[1];
	//glm_vec3_add(player->velocity, player->velUp, player->velocity);
	printf("Player velocity: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	//break the original velocity vector into front and right vector components, makes changes, then use unit vector from both and add together with scaling.


	//glm_vec3_copy(player->velocity, player->prev_velocity);

	//printf("vel adding? %f %f %f\n", player->velocity[0], dt * player->velocity[1], dt * player->velocity[2]);
	vec3 gravityHold = GLM_VEC3_ZERO_INIT;
	glm_vec3_add(player->velocity, player->velUp, gravityHold);

	player->coords[0] += dt * gravityHold[0];
	player->coords[1] += dt * gravityHold[1];
	player->coords[2] += dt * gravityHold[2];

	if (player->coords[1] < -20.0f) {
		player->velUp[1] = 0.0f;
		player->coords[0] = 0.0f;
		player->coords[1] = 5.0f;
		player->coords[2] = 0.0f;
	}

	glm_vec3_copy(player->coords, player->camera.cameraPos);
	updateOBBPos(&player->box, player->coords);
	glm_vec3_copy(GLM_VEC3_ZERO, player->velAdded);
	//glm_vec3_copy(GLM_VEC3_ZERO, player->velMoveNormal);
	glm_vec3_copy(GLM_VEC3_ZERO, player->velMoveAir);
	player->is_colliding = false;
	printf("vel player after: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	//player->camera.cameraPos[1] = player->camera.cameraPos[1] + 0.5f;
}

void ApplyGroundResistance(vec3 vector, double dt) {
	//printf("applying vel resist on %f %f %f\n", vector[0], vector[1], vector[2]);
	vec3 tmp = GLM_VEC3_ZERO_INIT;
	glm_vec3_copy(vector, tmp);
	glm_normalize(tmp);
	glm_vec3_negate(tmp);
	glm_vec3_scale(tmp, dt * -GROUND_RESISTANCE, tmp);
	if (fabs(tmp[0]) >= fabs(vector[0])) {
		vector[0] = 0.0f;
	} else {
		vector[0] += tmp[0];	//sign1 * dt * AIR_GROUND_RESISTANCE;
	}


	if (fabs(tmp[2]) >= fabs(vector[2])) {
		vector[2] = 0.0f;
	} else {
		vector[2] += tmp[2];	//sign3 * dt * AIR_GROUND_RESISTANCE;
	}
	//printf("adding vel resist with %f %f %f\n", tmp[0], tmp[1], tmp[2]);
}

void ApplyAirResistance(vec3 vector, double dt) {
	vec3 tmp = GLM_VEC3_ZERO_INIT;
	glm_vec3_copy(vector, tmp);
	glm_normalize(tmp);
	glm_vec3_negate(tmp);
	glm_vec3_scale(tmp, dt * -AIR_RESISTANCE, tmp);
	if (fabs(tmp[0]) >= fabs(vector[0])) {
		vector[0] = 0.0f;
	} else {
		vector[0] += tmp[0];	//sign1 * dt * AIR_GROUND_RESISTANCE;
	}

	if (fabs(tmp[2]) >= fabs(vector[2])) {
		vector[2] = 0.0f;
	} else {
		vector[2] += tmp[2];	//sign3 * dt * AIR_GROUND_RESISTANCE;
	}
}

bool ComputeResolveCollisions(Player *player, Object *obj, float dt) {
	vec4 out = GLM_VEC4_ZERO_INIT;
	vec3 result = GLM_VEC3_ZERO_INIT;
	float max_move = CAM_SPEED * 2.5;

	player->velUpNormal[3] = 0;

	if (OOBvOBBCollisionSATStatic(&player->box, &obj->box, out) == true) {
		printf("vel up before changes is %f\n", player->velUp[1]);
		player->is_colliding = true;

		printf("collided with obj->id = %d\n", obj->ID);
		printf("obj->vel = %f, %f, %f\n", obj->velocity[0], obj->velocity[1], obj->velocity[2]);

		//change amount added...
		vec3 position_diff = GLM_VEC3_ZERO_INIT;
		glm_vec3_sub(player->coords, obj->coordinates, position_diff);
		float angle_diff = glm_vec3_angle(obj->velocity, position_diff);
		if (obj->velocity[0] != 0.0f && obj->velocity[1] != 0.0f && obj->velocity[2] != 0.0f) {
			if (position_diff[1] >= player->height || (angle_diff < 90.0f && position_diff[1] >= -player->height)) {	//player is on top relative to vel (height is the measure)
				glm_vec3_add(player->velAdded, obj->velocity, player->velAdded);
			}
		}

		//glm_vec3_add(player->velAdded, obj->velocity, player->velAdded);

		glm_vec3_copy(out, result);
		glm_vec3_scale(result, -out[3], result);


		glm_vec3_normalize(out);
		vec3 tmpNormalized = GLM_VEC3_ZERO_INIT;
		tmpNormalized[0] = out[0];	//(player->velUp[1] >= -0.0001f) ? 1.0f : -1.0f;
		tmpNormalized[2] = out[2];
		if (fabs(tmpNormalized[0]) == 0.0f && fabs(tmpNormalized[2]) == 0.0f) {
			tmpNormalized[1] = out[1];
		}
		glm_vec3_normalize(tmpNormalized);
		float sign = result[1];
		float angle = glm_deg(glm_vec3_angle(tmpNormalized, out));
		if (sign >= 0.0f && angle >= 45.0f) {	//glm_deg(glm_vec3_angle(tmpNormalized, out)) >= 50.0f //need to add more cases so normal coll works
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			tmp[1] = result[1];
			glm_vec3_add(player->coords, tmp, player->coords);
			glm_vec3_add(player->camera.cameraPos, tmp, player->camera.cameraPos);
			updateOBBPos(&player->box, player->coords);
		} else {
			glm_vec3_add(player->coords, result, player->coords);
			glm_vec3_add(player->camera.cameraPos, result, player->camera.cameraPos);
			updateOBBPos(&player->box, player->coords);
		}

		if (out[1] < 0.0f && player->velUp[1] < 0.0f && player->in_air == true) {	//&& player->velUp[1] < 0.0f
			//player->velocity[1] = 0.0f;
			//player->velUp[1] = 0.0f;
			player->in_air = false;
			player->velMove[0] = player->velocity[0];
			player->velMove[2] = player->velocity[2];
		} else if (out[1] < 0.0f && player->velUp[1] < 0.0f) {
			if (!player->jumping) {
				//player->velUp[1] = 0.0f;
				player->in_air = false;
			}
		}
		if (result[1] < 0.0f && player->velUp[1] > 0.0f) {
			//player->velocity[1] = 0.0f;
			//player->velUp[1] = 0.0f;
		}
		vec3 projectionOrthogonal = GLM_VEC3_ZERO_INIT;


		//general
		vec3 velIn = GLM_VEC3_ZERO_INIT;
		//printf("out: %f %f %f\n", out[0], out[1], out[2]);
		//printf("b4 forw: %f, %f, %f\n", player->velFoward[0], player->velFoward[1], player->velFoward[2]);
		//glm_vec3_add(player->velMove, player->velMoveAir, player->velMove);
		//glm_vec3_add(player->velMove, player->velAdded, player->velMove);

		if ((out[1]) < 0.0f) {	//now it's up (currently on incline)
			printf("case incline\n");
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			//glm_vec3_copy(out, tmp);
			//player->velMove[1] = 0.0f;	//negate the vertical to see... 7/19
			float angle_diff = glm_deg(glm_vec3_angle(out, player->velMove));
			glm_vec3_copy(player->velMove, player->velMoveNormal);
			printf("norm is same as velMove: %f, %f, %f\n", player->velMoveNormal[0], player->velMoveNormal[1], player->velMoveNormal[2]);

			if (fabs(player->velMove[0]) != 0.0f || fabs(player->velMove[2]) != 0.0f) {
				glm_vec3_cross(player->velMove, out, tmp);
				glm_vec3_rotate(player->velMoveNormal, glm_rad(angle_diff - 90.0f), tmp);
				printf("here?\n");
				printf("velMove in incline case: %f, %f, %f\n", player->velMoveNormal[0], player->velMoveNormal[1], player->velMoveNormal[2]);
				//player->velMoveNormal[3] = 1;	//true;
			}
			//player->velMove[1] = 0.0f;
		} else if ((out[1]) > 0.0f) {	//write it so that we ignore the y of out vector for our calc.
			printf("case below\n");

			vec3 tmp = GLM_VEC3_ZERO_INIT;
			glm_vec3_copy(out, tmp);
			tmp[1] = 0.0f;
			glm_vec3_normalize(tmp);
			if (glm_deg(glm_vec3_angle(tmp, player->velMove)) >= 90.0f) {
				glm_vec3_copy(player->velMove, player->velMoveNormal);
			} else {
				glm_vec3_proj(player->velMove, tmp, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				projectionOrthogonal[1] = 0.0f;
				glm_vec3_add(projectionOrthogonal, player->velMove, player->velMoveNormal);
				//glm_vec3_copy(player->velMoveNormal, player->velMove);
				//player->velMove[1] = 0.0f;
				//player->velMoveNormal[3] = 1;	//true;
			}
		} else {	//it's 0.0f
			if (glm_deg(glm_vec3_angle(out, player->velMove)) >= 90.0f) {
				printf("case zero 90 deg away\n");
				glm_vec3_copy(player->velMove, player->velMoveNormal);	//maybe rework
			} else {
				printf("case zero\n");
				glm_vec3_copy(GLM_VEC3_ZERO, projectionOrthogonal);
				glm_vec3_proj(player->velMove, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				glm_vec3_add(projectionOrthogonal, player->velMove, player->velMoveNormal);
				//player->velMoveNormal[3] = 1;	//true;
			}
		}
		printf("velMove: %f, %f, %f\n", player->velMove[0], player->velMove[1], player->velMove[2]);
		printf("after norm: %f, %f, %f\n", player->velMoveNormal[0], player->velMoveNormal[1], player->velMoveNormal[2]);

		//up
		printf("B4 vel up is %f\n", player->velUp[1]);
		//if (player->velUp[1] <= 0.0f) {
			if (sign >= 0.0f && angle >= 45.0f) { //need to think of way to have better incline physics...

				vec3 tmp = GLM_VEC3_ZERO_INIT;
				glm_vec3_copy(player->velUp, tmp);
				glm_vec3_proj(tmp, out, projectionOrthogonal);
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				projectionOrthogonal[0] = 0.0f;
				//projectionOrthogonal[1] /= 8.0f;	//think about it...
				projectionOrthogonal[2] = 0.0f;
				//glm_vec3_add(projectionOrthogonal, player->velUp, player->velUpNormal);
				glm_vec3_add(projectionOrthogonal, player->velUp, player->velUp);
				//glm_vec3_add(projectionOrthogonal, player->velUp, player->velUpNormal);
				if (!player->jumping) {
					//player->velUp[1] = 0.0f;
				}
				//player->jumping = false;
				//player->in_air = false;
			} else {
				glm_vec3_proj(player->velUp, out, projectionOrthogonal);
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				//glm_vec3_add(projectionOrthogonal, player->velUp, player->velUpNormal);
				glm_vec3_add(projectionOrthogonal, player->velUp, player->velUp);
				//glm_vec3_add(projectionOrthogonal, player->velUp, player->velUpNormal);
				//player->velUp[0] = 0.0f;
				//player->velUp[2] = 0.0f;
				//player->in_air = false;
				//player->jumping = true;
			}
		//}
		printf("BS vel up is %f\n", player->velUp[1]);
		player->velUpNormal[3] = 1.0f;

		if (player->jumping) {
			player->jumping = false;
		}
		printf("vel up modified is %f\n", player->velocity[1]);
		printf("in_air is == %d\n", player->in_air);
		//have player inherit velocity of object
		//*/


		return true;
	}
	return false;
}

void EvaluatePositionPlayerPostCollision(Player *player, double dt) {

}


/*	//saving for later.
		if ((out[1]) < 0.0f) {	//now it's up (currently on incline)
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			//glm_vec3_copy(out, tmp);
			float angle_diff = glm_deg(glm_vec3_angle(out, player->velFoward));
			glm_vec3_copy(player->velFoward, player->velFowardNormal);

			if (player->velFoward[0] != 0.0f || player->velFoward[2] != 0.0f) {
				glm_vec3_cross(player->velFoward, out, tmp);
				glm_vec3_rotate(player->velFowardNormal, glm_rad(angle_diff - 90.0f), tmp);
				player->velFowardNormal[3] = 1;	//true;
			}
		} else if ((out[1]) > 0.0f) {	//write it so that we ignore the y of out vector for our calc.
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			glm_vec3_copy(out, tmp);
			tmp[1] = 0.0f;
			glm_vec3_normalize(tmp);
			glm_vec3_proj(player->velFoward, tmp, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
			glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
			projectionOrthogonal[1] = 0.0f;
			glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFowardNormal);
			glm_vec3_copy(player->velFowardNormal, player->velFoward);
			player->velFowardNormal[3] = 1;	//true;
		} else {	//it's 0.0f
			if (glm_deg(glm_vec3_angle(out, player->velFoward)) >= 90.0f) {
				glm_vec3_copy(player->velFoward, player->velFowardNormal);
				printf("true!!!\n");
			} else {
				glm_vec3_copy(GLM_VEC3_ZERO, projectionOrthogonal);
				glm_vec3_proj(player->velFoward, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFowardNormal);
				player->velFowardNormal[3] = 1;	//true;
			}
		}
*/


