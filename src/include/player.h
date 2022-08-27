#ifndef PLAYER_H
#define PLAYER_H
#include "../include/camera.h"
#include "../include/OBB.h"

typedef struct Player {
	Camera camera;
	vec3 coords;	//derived from camera somewhat
	bool in_air;
	bool jumping;
	bool is_colliding;
	bool was_grounded;
	float mass;
	float width;
	float height;

	vec3 velocity;
	vec3 velMove;
	vec3 velAdded;

	vec3 velFoward;
	vec3 velLeft;
	vec3 velRight;
	vec3 velBack;
	vec3 velUp;

	vec4 velFowardNormal;
	vec4 velLeftNormal;
	vec4 velRightNormal;
	vec4 velBackNormal;
	vec4 velUpNormal;

	vec3 velMoveNormal;
	vec3 velMoveAir;
	vec3 velAddedNormal;

	vec3 prev_velocity;
	struct OBB box;
} Player;

Player initPlayer(vec3 coords);

#endif
