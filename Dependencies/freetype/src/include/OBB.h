#ifndef OBB_H
#define OBB_H

#include <stdio.h>
#include <stdbool.h>
#include "../../Dependencies/cglm/cglm.h" //<cglm/cglm.h>

typedef struct OBB {
	vec3 coord;
	float x_size;
	float y_size;
	float z_size;
	vec3 X;	//right
	vec3 Y;	//up
	vec3 Z;	//front
	vec3 orienation_axis;
	float rotation;
	vec3 points[8];	//8 since it's a cube
} OBB;

void initOBB(struct OBB *box, vec3 pos, float x, float y, float z, vec3 axis, float rot);
void updateOBB(struct OBB *box, vec3 pos, float x, float y, float z, vec3 axis, float rot);
void updateOBBPos(struct OBB *box, vec3 pos);
bool OOBvOBBCollisionSATStatic(struct OBB *box1, struct OBB *box2, vec4 result);	//result has axis and result[4] == distance back.
bool CheckAxis(vec3 pnts1[], vec3 pnts2[], vec3 axis, vec3 axisRes, float* depth);
bool CheckCrossAxis(vec3 pnts1[], vec3 pnts2[], vec3 axis1, vec3 axis2, vec3 axisRes, float* depth);

#endif
