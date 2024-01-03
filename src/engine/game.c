#include "game.h"

World_State world;
Player player;
const int TICK_RATE = 66;

// primitive data objects
	float square_vertices[] = { //x,y,z | x,y
		//front / back faces
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		//left / right faces
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 //bottom / top faces
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};

	int square_posIndices[] = {
		0, 1, 2,	//sq 1:
		1, 2, 3,

		4, 5, 6,	//sq 2:
		5, 6, 7,

		8, 9, 10,	//sq 3:
		9, 10, 11,

		12, 13, 14,	//sq 4:
		13, 14, 15,

		16, 17, 18,	//sq 5:
		17, 18, 19,

		20, 21, 22,	//sq 6:
		21, 22, 23
	};
	//*/
	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    	};

	float triangle_vertices[] = { //x,y,z | x,y
		//front / back faces
		 0.5f, -0.5f,  -0.5f, 	//bottom right | -z
		-0.5f, -0.5f,  -0.5f,  	//bottom left
		 0.0f,  0.5f,  0.0f,  	//top mid.

		 0.5f, -0.5f,  0.5f,  	//bottom right | +z
		-0.5f, -0.5f,  0.5f,  	//bottom left

		 //0.0f,  0.5f,  0.0f,  0.5f, 1.0f,	//top mid.	//redundant
		 0.5f, -0.5f,  -0.5f,  	//bottom right | -z
		-0.5f, -0.5f,  -0.5f,  	//bottom left
		 0.5f, -0.5f,  0.5f,  	//bottom right | +z
		-0.5f, -0.5f,  0.5f,  	//bottom left
	};

	int triangle_posIndices[] = {
		0, 1, 2,	//tri 1: back

		0, 3, 2,	//tri 2: right

		1, 4, 2,	//tri 3: left

		3, 4, 2,	//tri 4: front

		5, 6, 7,	//bottom piece 1:
		6, 8, 7		//bottom piece 2:
	};

void initWorld() {
	memset((void*)&world, 0, sizeof(world));
	world.objList = NULL;
	world.objCount = 0;
	world.objMax = 1;

	world.meshCount = 0;
	world.meshMax = 1;
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
		{5.0f, 1.0f, -5.0f}
	};

	addObj(meshType_cube_simple, NULL, false, false, objPositions[0], (vec3){20.0f, 0.1f, 20.0f}, (vec3){1.0f, 0.0f, 0.0f}, 0.0f, 0, NULL);

	for (int i = 1; i < 5; i++) {
		//if (i % 2 == 0) {
			//addObj(meshType_triangle, true, objPositions[i], (vec3){1.5f, 1.0f, 1.5f}, (vec3){0.0f, 1.0f, 0.0f}, rand() % 91, i);
		//} else {
		if (i == 1) {
			addObj(meshType_cube_simple, NULL, true, false, objPositions[i], (vec3){1.0f, 1.0f, 1.0f}, (vec3){0.0f, 1.0f, 0.0f}, 45.0f, i, NULL);
		} else {
			vec3 tmp = {0.0f, 0.1f, 0.0f};
			glm_vec3_add(tmp, objPositions[i], objPositions[i]);
			addObj(meshType_cube_simple, NULL, true, false, objPositions[i], (vec3){3.0f, 1.0f, 3.0f}, (vec3){(rand() % 11)/10.0f, 0.0f, (rand() % 11)/10.0f}, (rand() % 50)/1.0f, i, &updateObjVelFuncXZCircle);
		}
		//}
	}
	//add last for debug testing
	addObj(meshType_cube_simple, NULL, true, false, (vec3){-5.5f, 0.8f, -3.5f}, (vec3){3.2f, 1.0f, 3.2f}, (vec3){1.0f, 0.0f, 0.0f}, 45.0f, 5, &updateObjVelFuncXZCircle);
	
	//add light source.
	addObj(meshType_cube_light, NULL, true, true, (vec3){0.0f, 4.0f, 0.0f}, (vec3){1.2f, 1.2f, 1.2f}, (vec3){0.0f, 1.0f, 0.0f}, 0.0f, 6, NULL);
	
	//another light source.
	addObj(meshType_cube_light, NULL, true, true, (vec3){10.0f, -1.0f, 10.0f}, (vec3){3.0f, 1.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, 0.0f, 7, &updateObjVelFuncVertical);
	
	//extra
	int res = addObj(meshType_cube_simple, NULL, true, false, (vec3){12.0f, 1.0f, 1.0f}, (vec3){1.5f, 1.50f, 1.5f}, (vec3){0.0f, 1.0f, 0.0f}, 0.0f, 8, &updateObjVelFuncXZCircle);
	printf("double checking res is %d\n", res);
	//world.objList[8].lightSrc = false;

	//extra
	addObj(meshType_OBJ_light, "Resources/Models/teapot.obj", true, false, (vec3){0.0f, 2.3f, 7.0f}, (vec3){0.5f, 0.5f, 0.5f}, (vec3){0.0f, 1.0f, 0.0f}, 0.0f, 9, NULL);
	world.objList[9].hasCollision = false;
	
	checkObjList(&player);
}

int addObj(meshType t, char* path, bool one_txt, bool isLightSource, vec3 coords, vec3 scale, vec3 rot_axis, float angle, unsigned int index, void (*f)(float, float, float, vec3)) {
	world.objCount++;
	if (world.objMax <= world.objCount) {
		world.objMax *= 2;
		world.objList = (Object*) realloc(world.objList, sizeof(Object) * world.objMax);
	}
	Object ex;
	ex.ID = index;
	ex.type = t;
	ex.one_txture = one_txt;
	ex.hasCollision = true;
	ex.lightSrc = isLightSource;
	glm_vec3_copy(coords, ex.coordinates);
	glm_vec3_copy(scale, ex.scale_dim);
	glm_vec3_copy(rot_axis, ex.orientation_axis);
	ex.rotation = angle;
	//glm_vec3_copy(GLM_VEC3_ZERO, ex.velocity);
	//OBB
	initOBB(&ex.box, coords, scale[0]/2.0f, scale[1]/2.0f, scale[2]/2.0f, ex.orientation_axis, angle);
	ex.velFunc = f;	//&updateObjVelFuncXZCircle	//updateObjVelFuncLinearFlat; //&updateObjVelFuncLinearFlat; //updateObjVelFuncXZCircle //updateObjVelFuncLinearFlat;
	glm_vec3_zero(ex.velocity);
	glm_vec3_zero(ex.futureVel);

	int meshIdx = addMesh(t, path);
	ex.meshIdx = meshIdx;

	world.objList[world.objCount - 1] = ex;	//(Object){.type = t, .one_txture = one_txt, .coordinates = coords, .scale_dim = scale};
	printf("Added object id: %d\n", world.objCount - 1);
	return world.objCount - 1;
}

//returns mesh index
int addMesh(meshType type, char* path) {
	if (path != NULL) {
		for (int i = 0; i < world.meshCount; i++) {
			if (strcmp(path, world.meshList[i].PATH) == 0) {	//match path name so we say the are the same mesh
				// it's a dup just return it's index saying we "added it"
				return i;
			}
		}
	}
	int meshIdx = world.meshCount;
	world.meshCount++;
	if (world.meshMax <= world.meshCount) {
		world.meshMax *= 2;
		world.meshList = (Mesh*) realloc(world.meshList, sizeof(Mesh) * world.meshMax);
	}

	if (path != NULL) {
		strncpy(world.meshList[meshIdx].PATH, path, sizeof(world.meshList[meshIdx].PATH));	// limit is 4096
	} else {
		memset(world.meshList[meshIdx].PATH, 0, sizeof(world.meshList[meshIdx].PATH));
	}

	float* data;
	unsigned int data_len;
	int* indices;
	unsigned int indices_len;
	//readObjFile(path, sizeof(path), &data, (unsigned int*)&data_len, &indices, (unsigned int*)&indices_len);
	printf("hello guys why tf can't we do anything right??\n");
	switch (type) {
		case meshType_cube_simple:
			printf("hello guys why tf can't we do anything right?? x3\n");
			setupSimpleMesh(&(world.meshList[meshIdx]), vertices, sizeof(vertices), NULL, 0);
			break;
		case meshType_cube_light:
			setupSimpleMesh(&(world.meshList[meshIdx]), vertices, sizeof(vertices), NULL, 0);
			break;
		case meshType_OBJ_simple:
			readObjFile(path, strlen(path), &data, (unsigned int*)&data_len, &indices, (unsigned int*)&indices_len);
			setupMesh(&(world.meshList[meshIdx]), &data, data_len, &indices, indices_len);
			break;
		case meshType_OBJ_light:
			readObjFile(path, strlen(path), &data, (unsigned int*)&data_len, &indices, (unsigned int*)&indices_len);
			setupMesh(&(world.meshList[meshIdx]), &data, data_len, &indices, indices_len);
			break;
		default:
			printf("lol should not be here\n");
			break;
	}
	printf("hello guys why tf can't we do anything right?? x44\n");
	return meshIdx;
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

void updateObjVelFuncVertical(float float_tick, float offset, float r, vec3 ret) {
	glm_vec3_copy((vec3){0.0f, -r * sin(glm_rad(float_tick + offset)), 0.0f}, ret);
}


void checkObjList(Player *plyr) {
	plyr->objCollisionList = (bool*) realloc(plyr->objCollisionList, sizeof(bool) * world.objMax);
	for (int i = 0; i < world.objMax; i++) {
		plyr->objCollisionList[i] = false;
	}
}

void updateVelAdded(Player *plyr) {
	glm_vec3_zero(plyr->velAdded);
	for (int i = 0; i < world.objMax; i++) {
		if (plyr->objCollisionList[i] == true) {
			glm_vec3_add(plyr->velAdded, world.objList[i].velocity, plyr->velAdded);
		}
		plyr->objCollisionList[i] = false;
	}
}

Object* findClosestLightSrc(Object obj, int* count) {	//should use coarse grid for simple searches, temp solution for now.
	Object* closest_src_list = (Object*) malloc(sizeof(Object) * 5);
	int index = 0;
	for (int i = 0; i < world.objCount; i++) {
		if (world.objList[i].lightSrc == true) {
			closest_src_list[index] = world.objList[i];
			index++;
		}
	}
	//will add size checks/closest light later...
	*count = index;
	return closest_src_list;
}

