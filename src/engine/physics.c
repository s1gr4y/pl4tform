#include "physics.h"

const double GRAVITY = -0.3f/66.0f;
const double AIR_RESISTANCE = -0.01f/66.0f;	//-0.1f/66.0f;
const double GROUND_RESISTANCE = -0.15f/66.0f;	//-0.1f/66.0f;

void ComputePositionObj(Object obj) {
	exit(-1);
}

void ComputePositionPlayer(Player *player, double dt) {
	printf("vel player start-edt: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	if (player->in_air == false) {
		ApplyGroundResistance(player->velFoward, dt);
		ApplyGroundResistance(player->velRight, dt);
		ApplyGroundResistance(player->velLeft, dt);
		ApplyGroundResistance(player->velBack, dt);
		ApplyGroundResistance(player->velUp, dt);
	} else {
		ApplyAirResistance(player->velFoward, dt);
		ApplyAirResistance(player->velFowardNormal, dt);
		ApplyAirResistance(player->velRight, dt);
		ApplyAirResistance(player->velRightNormal, dt);
		ApplyAirResistance(player->velLeft, dt);
		ApplyAirResistance(player->velLeftNormal, dt);
		ApplyAirResistance(player->velBack, dt);
		ApplyAirResistance(player->velBackNormal, dt);
		ApplyAirResistance(player->velUp, dt);
	}
	float magnitudeFront = glm_vec3_norm(player->velFoward);
	float magnitudeSideR = glm_vec3_norm(player->velRight);
	float magnitudeBack = glm_vec3_norm(player->velBack);
	float magnitudeSideL = glm_vec3_norm(player->velLeft);
	//printf("dt: %f\n", dt);
	float maxFoward =  CAM_SPEED * 2.5;	//(player->in_air) ? CAM_SPEED * 0.5f: CAM_SPEED * 2.5;
	float maxSide =  CAM_SPEED * 2.25;	//(player->in_air) ? CAM_SPEED * 0.5f: CAM_SPEED * 2.25;
	float magTot = maxFoward;	//sqrtf(maxFoward + maxSide);
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

	//glm_vec3_add(player->velFoward, player->velBack, player->velFront);
	//glm_vec3_add(player->velLeft, player->velRight, player->velSide);

	vec3 movementSum = GLM_VEC3_ZERO_INIT;
	if (player->velFowardNormal[3] > 0) {
		glm_vec3_add(movementSum, player->velFowardNormal, movementSum);
	} else {
		glm_vec3_add(movementSum, player->velFoward, movementSum);
	}
	if (player->velBackNormal[3] > 0) {
		glm_vec3_add(movementSum, player->velBackNormal, movementSum);
	} else {
		glm_vec3_add(movementSum, player->velBack, movementSum);
	}
	if (player->velLeftNormal[3] > 0) {
		glm_vec3_add(movementSum, player->velLeftNormal, movementSum);
	} else {
		glm_vec3_add(movementSum, player->velLeft, movementSum);
	}
	if (player->velRightNormal[3] > 0) {
		glm_vec3_add(movementSum, player->velRightNormal, movementSum);
	} else {
		glm_vec3_add(movementSum, player->velRight, movementSum);
	}


	vec3 rightVecProj = GLM_VEC3_ZERO_INIT;
	vec3 fowardVecProj = GLM_VEC3_ZERO_INIT;
	vec3 outRightProj = GLM_VEC3_ZERO_INIT;
	vec3 outFowardProj = GLM_VEC3_ZERO_INIT;
	rightVecProj[0] = movementSum[0]*cos(glm_rad(90.0f)) - movementSum[2]*sin(glm_rad(90.0f));
	rightVecProj[2] = movementSum[0]*sin(glm_rad(90.0f)) + movementSum[2]*cos(glm_rad(90.0f));
	glm_vec3_copy(movementSum, fowardVecProj);
	fowardVecProj[1] = 0.0f;
	//need new speed limit solution...

	if (magTot < glm_vec3_norm(movementSum)) {
		glm_vec3_normalize(movementSum);
		glm_vec3_scale(movementSum, magTot, movementSum);
	}

	//if (magTot >= glm_vec3_norm(movementSum)) {
	//	printf("what?!?!?!?\n");
		glm_vec3_copy(movementSum, player->velocity);
	//}
	/*
	if (player->in_air == false) {
		ApplyGroundResistance(player->velocity, dt);
	} else {
		ApplyAirResistance(player->velocity, dt);
	}
	*/

	if (player->velUp[1] < 0.0f && fabs(player->velUp[1]) >= fabs(GRAVITY * 40)) {
		//printf("%f >= %f\n",player->velocity[1], GRAVITY * 100);
		player->velUp[1] = GRAVITY * 40;
	} else {
		player->velUp[1] += dt * GRAVITY;
	}
	//if (player->velUpNormal[3] > 0.0f) {
	//	printf("why??????????\n");
	//	glm_vec3_add(player->velocity, player->velUpNormal, player->velocity);
	//} else {
		glm_vec3_add(player->velocity, player->velUp, player->velocity);
	//}
	//break the original velocity vector into front and right vector components, makes changes, then use unit vector from both and add together with scaling.


	//glm_vec3_copy(player->velocity, player->prev_velocity);

	//printf("vel adding? %f %f %f\n", player->velocity[0], dt * player->velocity[1], dt * player->velocity[2]);
	player->coords[0] += dt * player->velocity[0];
	player->coords[1] += dt * player->velocity[1];
	player->coords[2] += dt * player->velocity[2];

	//if (player->coords[1] <= -1.00f) {
	//	player->coords[1] = -1.00f;
	//	player->velocity[1] = 0.0f;
	//	player->velUp[1] = 0.0f;
	//	player->in_air = false;
	//}
	printf("in vel player post-edt: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
	printf("player->velFoward: %f %f %f\n", player->velFoward[0], player->velFoward[1], player->velFoward[2]);
	printf("player->velRight: %f %f %f\n", player->velRight[0], player->velRight[1], player->velRight[2]);
	printf("player->velLeft: %f %f %f\n", player->velLeft[0], player->velLeft[1], player->velLeft[2]);
	printf("player->velBack: %f %f %f\n", player->velBack[0], player->velBack[1], player->velBack[2]);
	printf("player->velUp: %f %f %f\n", player->velUp[0], player->velUp[1], player->velUp[2]);
	if (player->coords[1] < -20.0f) {
		player->velUp[1] = 0.0f;
		player->coords[0] = 0.0f;
		player->coords[1] = 5.0f;
		player->coords[2] = 0.0f;
	}

	glm_vec3_copy(player->coords, player->camera.cameraPos);
	updateOBBPos(&player->box, player->coords);
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

	player->velUpNormal[3] = 0;

	if (OOBvOBBCollisionSATStatic(&player->box, &obj->box, out) == true) {
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
		if (result[1] > 0.0f && player->velUp[1] < 0.0f) {
			//player->velocity[1] = 0.0f;
			//player->velUp[1] = 0.0f;
			player->in_air = false;
		}
		if (result[1] < 0.0f && player->velUp[1] > 0.0f) {
			//player->velocity[1] = 0.0f;
			//player->velUp[1] = 0.0f;
		}
		///*
		vec3 projectionOrthogonal = GLM_VEC3_ZERO_INIT;
		//glm_vec3_proj(player->velocity, out, projectionOrthogonal);
		//glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
		//glm_vec3_add(projectionOrthogonal, player->velocity, player->velocity);

		//forward
		vec3 tmp1 = GLM_VEC3_ZERO_INIT;
		//glm_vec3_negate(out);
		//out[0] = -out[0];
		//out[1] = -out[1];
		//out[2] = -out[2];
		printf("out: %f %f %f\n", out[0], out[1], out[2]);
		printf("b4 forw: %f, %f, %f\n", player->velFoward[0], player->velFoward[1], player->velFoward[2]);

		//fine algo (cap lmao) but need to figure out how to deal with micro non collisions
		///*
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
				glm_vec3_proj(player->velFoward, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFowardNormal);
				player->velFowardNormal[3] = 1;	//true;
			}
		}
		//*/
		//glm_vec3_proj(player->velFoward, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
		//glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
		//glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFoward);
		//glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFowardNormal);
		player->velFowardNormal[3] = 1;	//true;
		if (sign < 0.0f) {
			//player->velFoward[1] = 0.0f;
			//player->velFowardNormal[1] = 0.0f;
		}
		printf("now forw: %f, %f, %f\n", player->velFowardNormal[0], player->velFowardNormal[1], player->velFowardNormal[2]);

		//back

		if ((out[1]) < 0.0f) {	//now it's up (currently on incline)
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			//glm_vec3_copy(out, tmp);
			float angle_diff = glm_deg(glm_vec3_angle(out, player->velBack));
			glm_vec3_copy(player->velBack, player->velBackNormal);

			if (player->velBack[0] != 0.0f || player->velBack[2] != 0.0f) {
				glm_vec3_cross(player->velBack, out, tmp);
				glm_vec3_rotate(player->velBackNormal, glm_rad(angle_diff - 90.0f), tmp);
				player->velBackNormal[3] = 1;	//true;
			}
		} else if ((out[1]) > 0.0f) {	//write it so that we ignore the y of out vector for our calc.
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			glm_vec3_copy(out, tmp);
			tmp[1] = 0.0f;
			glm_vec3_normalize(tmp);
			glm_vec3_proj(player->velBack, tmp, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
			glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
			projectionOrthogonal[1] = 0.0f;
			glm_vec3_add(projectionOrthogonal, player->velBack, player->velBackNormal);
			glm_vec3_copy(player->velBackNormal, player->velBack);
			player->velBackNormal[3] = 1;	//true;
		} else {	//it's 0.0f
			if (glm_deg(glm_vec3_angle(out, player->velBack)) >= 90.0f) {
				glm_vec3_copy(player->velBack, player->velBackNormal);
				printf("true!!!\n");
			} else {
				glm_vec3_proj(player->velBack, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				glm_vec3_add(projectionOrthogonal, player->velBack, player->velBackNormal);
				player->velBackNormal[3] = 1;	//true;
			}
		}

		//glm_vec3_proj(player->velBack, out, projectionOrthogonal);
		//glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
		//glm_vec3_add(projectionOrthogonal, player->velBack, player->velBack);
		//glm_vec3_add(projectionOrthogonal, player->velBack, player->velBackNormal);

		player->velBackNormal[3] = 1;

		//up
		printf("up: %f\n", player->velUp[1]);
		printf("tmpNormalized: %f %f %f\n", tmpNormalized[0], tmpNormalized[1], tmpNormalized[2]);
		printf("out: %f %f %f\n", out[0], out[1], out[2]);
		printf("result: %f %f %f\n", result[0], result[1], result[2]);
		printf("res: %f\n", glm_deg(glm_vec3_angle(tmpNormalized, out)));
		if (player->jumping == true) {
			player->jumping = false;
			player->in_air = true;
		}
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
			player->velUp[1] = 0.0f;
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
		}
		player->velUpNormal[3] = 1.0f;


		//left
		if ((out[1]) < 0.0f) {	//now it's up (currently on incline)
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			//glm_vec3_copy(out, tmp);
			float angle_diff = glm_deg(glm_vec3_angle(out, player->velLeft));
			glm_vec3_copy(player->velLeft, player->velLeftNormal);

			if (player->velLeft[0] != 0.0f || player->velLeft[2] != 0.0f) {
				//if (angle_diff > 90.0f) {
				glm_vec3_cross(player->velLeft, out, tmp);
				glm_vec3_rotate(player->velLeftNormal, glm_rad(angle_diff - 90.0f), tmp);
				//player->velFowardNormal[1] = 4.0f*(player->velFowardNormal[1]);
				player->velLeftNormal[3] = 1;	//true;
				printf("ROTATED by %f!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", (angle_diff - 90.0f));
			}
		} else {	//it's 0.0f
			if (glm_deg(glm_vec3_angle(out, player->velLeft)) >= 90.0f) {
				glm_vec3_copy(player->velLeft, player->velLeftNormal);
				printf("true!!!\n");
			} else {
				glm_vec3_proj(player->velLeft, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				//glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFoward);
				if ((out[1]) > 0.0f) {
					projectionOrthogonal[1] = 0.0f;
				}
				glm_vec3_add(projectionOrthogonal, player->velLeft, player->velLeftNormal);
				glm_vec3_copy(player->velLeftNormal, player->velLeft);
				player->velLeftNormal[3] = 1;	//true;
			}
		}


		//printf("left PRE res: %f, %f, %f\n", player->velLeft[0], player->velLeft[1], player->velLeft[2]);
		//glm_vec3_proj(player->velLeft, out, projectionOrthogonal);
		//glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
		//glm_vec3_add(projectionOrthogonal, player->velLeft, player->velLeft);

		printf("left post res: %f, %f, %f\n", player->velLeft[0], player->velLeft[1], player->velLeft[2]);

		//right
		if ((out[1]) < 0.0f) {	//now it's up (currently on incline)
			vec3 tmp = GLM_VEC3_ZERO_INIT;
			//glm_vec3_copy(out, tmp);
			float angle_diff = glm_deg(glm_vec3_angle(out, player->velRight));
			glm_vec3_copy(player->velRight, player->velRightNormal);

			if (player->velRight[0] != 0.0f || player->velRight[2] != 0.0f) {
				//if (angle_diff > 90.0f) {
				glm_vec3_cross(player->velRight, out, tmp);
				glm_vec3_rotate(player->velRightNormal, glm_rad(angle_diff - 90.0f), tmp);
				//player->velFowardNormal[1] = 4.0f*(player->velFowardNormal[1]);
				player->velRightNormal[3] = 1;	//true;
				printf("ROTATED by %f!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", (angle_diff - 90.0f));
			}
		} else {	//it's 0.0f
			if (glm_deg(glm_vec3_angle(out, player->velRight)) >= 90.0f) {
				glm_vec3_copy(player->velRight, player->velRightNormal);
				printf("true!!!\n");
			} else {
				glm_vec3_proj(player->velRight, out, projectionOrthogonal);	//glm_vec3_proj(v, u, out)	// proj_u(v) == out
				glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
				//glm_vec3_add(projectionOrthogonal, player->velFoward, player->velFoward);
				if ((out[1]) > 0.0f) {
					projectionOrthogonal[1] = 0.0f;
				}
				glm_vec3_add(projectionOrthogonal, player->velRight, player->velRightNormal);
				glm_vec3_copy(player->velRightNormal, player->velRight);
				player->velRightNormal[3] = 1;	//true;
			}
		}

		//glm_vec3_proj(player->velRight, out, projectionOrthogonal);
		//glm_vec3_negate(projectionOrthogonal);	//need opposite to add back.
		//glm_vec3_add(projectionOrthogonal, player->velRight, player->velRight);

		printf("vel player post-edt: %f, %f, %f\n", player->velocity[0], player->velocity[1], player->velocity[2]);
		printf("player->velFoward: %f %f %f\n", player->velFoward[0], player->velFoward[1], player->velFoward[2]);
		printf("player->velRight: %f %f %f\n", player->velRight[0], player->velRight[1], player->velRight[2]);
		printf("player->velLeft: %f %f %f\n", player->velLeft[0], player->velLeft[1], player->velLeft[2]);
		printf("player->velBack: %f %f %f\n", player->velBack[0], player->velBack[1], player->velBack[2]);
		printf("player->velUp: %f %f %f\n", player->velUp[0], player->velUp[1], player->velUp[2]);
		printf("player->velFowardNormal: %f %f %f\n", player->velFowardNormal[0], player->velFowardNormal[1], player->velFowardNormal[2]);
		printf("player->velRightNormal: %f %f %f\n", player->velRightNormal[0], player->velRightNormal[1], player->velRightNormal[2]);
		printf("player->velLeftNormal: %f %f %f\n", player->velLeftNormal[0], player->velLeftNormal[1], player->velLeftNormal[2]);
		printf("player->velBackNormal: %f %f %f\n", player->velBackNormal[0], player->velBackNormal[1], player->velBackNormal[2]);
		printf("player->velUpNormal: %f %f %f\n", player->velUpNormal[0], player->velUpNormal[1], player->velUpNormal[2]);
		//have player inherit velocity of object
		//*/


		return true;
	}
	return false;
}

void EvaluatePositionPlayerPostCollision(Player *player, double dt) {

}


