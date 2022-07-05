#ifndef GAME_H
#define GAME_H

#include "../utils/mesh.h"
#include "../entity/player.h"
#include "physics.h"
#include "../entity/player.h"

typedef struct World_State {
	struct Object *objList;
	unsigned int objCount;
	unsigned int listMax;
} World_State;

void initWorld();
void addObj(meshType t, bool one_txt, vec3 coords, vec3 scale, vec3 rot_axis, float angle, unsigned int index);

extern World_State world;
extern Player player;
extern const int TICK_RATE;

#endif
