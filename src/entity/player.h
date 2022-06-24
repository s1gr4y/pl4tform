#ifndef PLAYER_H
#define PLAYER_H
#include "camera.h"

typedef struct Player {
	Camera camera;
	vec3 coords;	//derived from camera somewhat
	bool in_air;
	float mass;
	vec3 acceleration;
	vec3 velocity;
	vec3 velFoward;
	vec3 velLeft;
	vec3 velRight;
	vec3 velBack;
	vec3 velUp;
	vec3 prev_velocity;
} Player;

Player initPlayer(vec3 coords);

#endif
