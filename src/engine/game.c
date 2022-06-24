#include "game.h"

World_State world;
Player player;
const int TICK_RATE = 66;

void initWorld() {
	world.objList = NULL;
	world.objCount = 0;
	world.listMax = 1;

	player.mass = 10.0f;	//does nothing for now.
	Camera_init(&player.camera);
	glm_vec3_copy(player.camera.cameraPos, player.coords);
	player.coords[1] = player.camera.cameraPos[1] - 0.5f;
	glm_vec3_zero(player.velocity);
	glm_vec3_zero(player.velFoward);
	glm_vec3_zero(player.velLeft);
	glm_vec3_zero(player.velRight);
	glm_vec3_zero(player.velBack);
	glm_vec3_zero(player.velUp);
	glm_vec3_zero(player.prev_velocity);

	vec3 objPositions[9] = {	//figure out game objects / collision / try triangle - done
	    { 0.0f, -1.55f,  0.0f},
	    { 3.0f, -1.0f, -3.0f},
	    {-1.5f, -1.0f, -1.5f},
	    {2.0f, -1.0f, -1.0f},
		{2.0f, -1.0f, 2.0f},
		{4.0f, -1.0f, 1.0f},
		{-1.5f, -1.0f, 1.0f},
		{1.0f, -1.0f, 4.0f},
		{-4.0f, -1.0f, 4.0f}
	};

	addObj(meshType_cube, false, objPositions[0], (vec3){10.0f, 0.1f, 10.0f}, (vec3){0.0f, 0.0f, 0.0f}, 0.0f);

	for (int i = 1; i < 9; i++) {
		if (i % 2 == 0) {
			addObj(meshType_triangle, true, objPositions[i], (vec3){1.5f, 1.0f, 1.5f}, (vec3){0.0f, 1.0f, 0.0f}, rand() % 91);
		} else {
			addObj(meshType_cube, true, objPositions[i], (vec3){1.0f, 1.0f, 1.0f}, (vec3){0.0f, 0.0f, 0.0f}, 0.0f);
		}
	}
}

void addObj(meshType t, bool one_txt, vec3 coords, vec3 scale, vec3 rot_axis, float angle) {
	world.objCount++;
	if (world.listMax <= world.objCount) {
		world.listMax *= 2;
		world.objList = (Object*) realloc(world.objList, sizeof(Object) * world.listMax);
	}
	Object ex;
	ex.type = t;
	ex.one_txture = one_txt;
	glm_vec3_copy(coords, ex.coordinates);
	glm_vec3_copy(scale, ex.scale_dim);
	glm_vec3_copy(rot_axis, ex.orientation_axis);
	ex.rotation = angle;
	world.objList[world.objCount - 1] = ex;	//(Object){.type = t, .one_txture = one_txt, .coordinates = coords, .scale_dim = scale};
}
