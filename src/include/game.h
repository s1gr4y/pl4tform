#ifndef GAME_H
#define GAME_H

#include "../include/mesh.h"
#include "../include/player.h"
#include "../include/physics.h"

typedef struct World_State {
	struct Object* objList;
	unsigned int objCount;
	unsigned int objMax;

	struct Mesh* meshList;
	unsigned int meshCount;
	unsigned int meshMax;
} World_State;

void initWorld();
int addObj(meshType t, char* path, bool one_txt, bool isLightSource, vec3 coords, vec3 scale, vec3 rot_axis, float angle, unsigned int index, void (*f)(float, float, float, vec3));
void updateObjPos(Object* obj, float dt, float float_tick);
void updateObjVel(Object* obj, float dt, float float_tick);
void updateObjVelFuncXZCircle(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncXYZCircle(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncLinear(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncLinearFlat(float float_tick, float offset, float r, vec3 ret);
void updateObjVelFuncVertical(float float_tick, float offset, float r, vec3 ret);

Object* findClosestLightSrc(Object obj, int* count);

//unused rn
void checkObjList(Player *plyr);
void updateVelAdded(Player *plyr);

extern World_State world;
extern Player player;
extern const int TICK_RATE;

#endif
