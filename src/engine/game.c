#include "game.h"

World_State world;
Player player;
const int TICK_RATE = 66;

void initWorld() {
	world.objList = NULL;
	world.objCount = 0;
	world.listMax = 1;
	player.resetVelAdded = false;

	player.mass = 10.0f;	//does nothing for now.
	Camera_init(&player.camera);
	glm_vec3_copy(player.camera.cameraPos, player.coords);
	//player.coords[1] = player.camera.cameraPos[1] - 0.5f;
	glm_vec3_zero(player.velocity);
	glm_vec3_zero(player.velFoward);
	glm_vec3_zero(player.velLeft);
	glm_vec3_zero(player.velRight);
	glm_vec3_zero(player.velBack);
	glm_vec3_zero(player.velUp);
	glm_vec3_zero(player.prev_velocity);

	glm_vec4_zero(player.velFowardNormal);
	glm_vec3_zero(player.velLeftNormal);
	glm_vec3_zero(player.velRightNormal);
	glm_vec3_zero(player.velBackNormal);
	glm_vec3_zero(player.velUpNormal);
	glm_vec3_zero(player.velMoveAir);
	glm_vec3_zero(player.velMoveNormal);

	player.width = 0.25f;
	player.height = 0.5f;
	initOBB(&player.box, player.coords, player.width, player.height, player.width, (vec3){0.0f, 1.0f, 0.0f}, 90.0f);

	vec3 objPositions[] = {	//figure out game objects / collision / try triangle - done
	    { 0.0f, 0.45f,  0.0f},
	    { 0.0f, 1.0f, 0.0f},
	    {5.0f, 1.0f, 5.0f},
	    {-5.0f, 1.0f, 5.0f},
		{5.0f, 1.0f, -5.0f},
	};

	addObj(meshType_cube, false, objPositions[0], (vec3){20.0f, 0.1f, 20.0f}, (vec3){1.0f, 0.0f, 0.0f}, 0.0f, 0);

	for (int i = 1; i < 5; i++) {
		//if (i % 2 == 0) {
			//addObj(meshType_triangle, true, objPositions[i], (vec3){1.5f, 1.0f, 1.5f}, (vec3){0.0f, 1.0f, 0.0f}, rand() % 91, i);
		//} else {
		if (i == 1) {
			addObj(meshType_cube, true, objPositions[i], (vec3){1.0f, 1.0f, 1.0f}, (vec3){0.0f, 1.0f, 0.0f}, 45.0f, i);
		} else {
			vec3 tmp = {0.0f, 0.1f, 0.0f};
			glm_vec3_add(tmp, objPositions[i], objPositions[i]);
			addObj(meshType_cube, true, objPositions[i], (vec3){3.0f, 1.0f, 3.0f}, (vec3){(rand() % 11)/10.0f, 0.0f, (rand() % 11)/10.0f}, (rand() % 50)/1.0f, i);
		}
		//}
	}
	//add last for debug testing
	addObj(meshType_cube, true, (vec3){-5.5f, 0.8f, -3.5f}, (vec3){3.2f, 1.0f, 3.2f}, (vec3){1.0f, 0.0f, 0.0f}, 45.0f, 6);
	
	checkObjList(&player);
}

void addObj(meshType t, bool one_txt, vec3 coords, vec3 scale, vec3 rot_axis, float angle, unsigned int index) {
	world.objCount++;
	if (world.listMax <= world.objCount) {
		world.listMax *= 2;
		world.objList = (Object*) realloc(world.objList, sizeof(Object) * world.listMax);
	}
	Object ex;
	ex.ID = index;
	ex.type = t;
	ex.one_txture = one_txt;
	glm_vec3_copy(coords, ex.coordinates);
	glm_vec3_copy(scale, ex.scale_dim);
	glm_vec3_copy(rot_axis, ex.orientation_axis);
	ex.rotation = angle;
	//glm_vec3_copy(GLM_VEC3_ZERO, ex.velocity);
	//OBB
	initOBB(&ex.box, coords, scale[0]/2.0f, scale[1]/2.0f, scale[2]/2.0f, ex.orientation_axis, angle);
	if (one_txt == true) {
		ex.velFunc = &updateObjVelFuncXZCircle; //&updateObjVelFuncLinearFlat;	//updateObjVelFuncXZCircle	//updateObjVelFuncLinearFlat;
		//ex.velFunc = NULL;
	} else {
		ex.velFunc = NULL;
	}
	glm_vec3_zero(ex.velocity);
	glm_vec3_zero(ex.futureVel);

	world.objList[world.objCount - 1] = ex;	//(Object){.type = t, .one_txture = one_txt, .coordinates = coords, .scale_dim = scale};
}

void updateObjPos(Object* obj, float dt, float float_tick) {	//to fix velocity post loop desync we need to have list of obj player got it's vel from then update it when it changes
	//needs other updates like obj vel update.
	//glm_vec3_copy(obj->futureVel, obj->velocity);
	//if (obj->velFunc != NULL) {	//post update, unsure if will do much but let's try
	//	obj->velFunc((360/(TICK_RATE*3))*fmod(floor(float_tick), (double) TICK_RATE*3), 0.0f, 0.02f, obj->velocity);
	//}

	obj->coordinates[0] += dt * TICK_RATE * obj->velocity[0];	//obj->velocity
	obj->coordinates[1] += dt * TICK_RATE * obj->velocity[1];
	obj->coordinates[2] += dt * TICK_RATE * obj->velocity[2];
	updateOBBPos(&obj->box, obj->coordinates);
}

void updateObjVel(Object* obj, float dt, float float_tick) {	//to fix velocity post loop desync we need to have list of obj player got it's vel from then update it when it changes
	//needs other updates like obj vel update.
	//glm_vec3_copy(obj->futureVel, obj->velocity);
	float interpVal = (360.0f/(float)(TICK_RATE*3));
	if (obj->velFunc != NULL) {	//post update, unsure if will do much but let's try
		obj->velFunc(interpVal*fmod(floor(float_tick), (float) TICK_RATE*3), 0.0f, 0.05f, obj->velocity);
		//printf("DEG OF ROTATION FOR MOVING OBJ IS %f\n", interpVal*fmod(floor(float_tick), (float) TICK_RATE*3), (float) TICK_RATE*3);	//interpVal*fmod(floor(float_tick), (float) TICK_RATE*3)
		//printf("sin of deg is... %f\n", sin(glm_rad(interpVal*fmod(floor(float_tick), (float) TICK_RATE*3))));
	}
}

void updateObjVelFuncXZCircle(float float_tick, float offset, float r, vec3 ret) { //circle xz axis
	glm_vec3_copy((vec3){r * cos(glm_rad(float_tick + offset)), 0.0f, r * sin(glm_rad(float_tick + offset))}, ret);
}

void updateObjVelFuncXYZCircle(float float_tick, float offset, float r, vec3 ret) {
	glm_vec3_copy((vec3){r * cos(glm_rad(float_tick + offset)), r * cos(glm_rad(float_tick + offset)), r * sin(glm_rad(float_tick + offset))}, ret);
}

void updateObjVelFuncLinear(float float_tick, float offset, float r, vec3 ret) {
	glm_vec3_copy((vec3){r * sin(glm_rad(float_tick + offset)), r * sin(glm_rad(float_tick + offset)), r * sin(glm_rad(float_tick + offset))}, ret);
}

void updateObjVelFuncLinearFlat(float float_tick, float offset, float r, vec3 ret) {
	glm_vec3_copy((vec3){r * sinf(glm_rad(float_tick + offset)), 0.0f, r * sinf(glm_rad(float_tick + offset))}, ret);
}


void checkObjList(Player *plyr) {
	plyr->objCollisionList = (bool*) realloc(plyr->objCollisionList, sizeof(bool) * world.listMax);
	for (int i = 0; i < world.listMax; i++) {
		plyr->objCollisionList[i] = false;
	}
}

void updateVelAdded(Player *plyr) {
	glm_vec3_zero(plyr->velAdded);
	for (int i = 0; i < world.listMax; i++) {
		if (plyr->objCollisionList[i] == true) {
			glm_vec3_add(plyr->velAdded, world.objList[i].velocity, plyr->velAdded);
		}
		plyr->objCollisionList[i] = false;
	}
}

