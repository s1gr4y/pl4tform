#ifndef GAME_H
#define GAME_H

#include "../include/mesh.h"
#include "../include/player.h"
#include "../include/physics.h"
#include "../include/player.h"

typedef struct World_State {
	struct Object *objList;
	unsigned int objCount;
	unsigned int listMax;
} World_State;

void initWorld();
void addObj(meshType t, bool one_txt, vec3 coords, vec3 scale, vec3 rot_axis, float angle, unsigned int index);
void updateObj(Object* obj, float dt, float float_tick);
void updateObjVelFuncXZCircle(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncXYZCircle(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncLinear(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncLinearFlat(float float_tick, float offset, float r, vec3 ret);

extern World_State world;
extern Player player;
extern const int TICK_RATE;

#endif
