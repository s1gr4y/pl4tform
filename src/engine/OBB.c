#include "OBB.h"

void initOBB(struct OBB *box, vec3 pos, float x, float y, float z, vec3 axis, float rot) {
	glm_vec3_copy(pos, box->coord);
	box->x_size = x;
	box->y_size = y;
	box->z_size = z;
	vec3 tmpPos = GLM_VEC3_ONE_INIT;
	vec3 right = {1.0f, 0.0f, 0.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	vec3 front = {0.0f, 0.0f, 1.0f};	//X,Y,Z vectors are default since I can always remember the rotation axis but not original vector pos.
	glm_vec3_copy(axis, box->orienation_axis);
	box->rotation = rot;
	versor q = GLM_QUAT_IDENTITY_INIT;
	glm_quatv(q, glm_rad(box->rotation), box->orienation_axis);
	//glm_quat_rotatev(q, tmpPos, tmpPos);
	glm_quat_rotatev(q, right, right);
	glm_quat_rotatev(q, up, up);
	glm_quat_rotatev(q, front, front);
	printf("right: %f, %f, %f\n", right[0], right[1], right[2]);
	printf("up: %f, %f, %f\n", up[0], up[1], up[2]);
	printf("front: %f, %f, %f\n", front[0], front[1], front[2]);
	//glm_vec3_rotate(right, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(up, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(front, glm_rad(box->rotation), box->orienation_axis);
	glm_vec3_normalize(right);
	glm_vec3_normalize(up);
	glm_vec3_normalize(front);
	glm_vec3_copy(right, box->X);
	glm_vec3_copy(up, box->Y);
	glm_vec3_copy(front, box->Z);

	//glm_vec3_scale()
	//glm_vec3_add()
	vec3 temp = GLM_VEC3_ZERO_INIT;
	temp[0] = 1.0f * box->x_size;
	temp[1] = 1.0f * box->y_size;
	temp[2] = 1.0f * box->z_size;

	vec3 max = GLM_VEC3_ZERO_INIT;
	vec3 min = GLM_VEC3_ZERO_INIT;
	vec3 out = GLM_VEC3_ZERO_INIT;

	max[0] = box->x_size;
	max[1] = box->y_size;
	max[2] = box->z_size;

	glm_vec3_copy(max, min);
	glm_vec3_negate(min);

	glm_vec3_copy((vec3){min[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[0]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[1]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[2]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[3]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[4]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[5]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[6]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[7]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);

}

void updateOBB(struct OBB *box, vec3 pos, float x, float y, float z, vec3 axis, float rot) {
	glm_vec3_copy(pos, box->coord);
	box->x_size = x;
	box->y_size = y;
	box->z_size = z;
	vec3 right = {1.0f, 0.0f, 0.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	vec3 front = {0.0f, 0.0f, 1.0f};	//X,Y,Z vectors are default since I can always remember the rotation axis but not original vector pos.
	glm_vec3_copy(axis, box->orienation_axis);
	box->rotation = rot;
	versor q = GLM_QUAT_IDENTITY_INIT;
	glm_quatv(q, glm_rad(box->rotation), box->orienation_axis);
	//glm_quat_rotatev(q, box->coord, box->coord);
	glm_quat_rotatev(q, right, right);
	glm_quat_rotatev(q, up, up);
	glm_quat_rotatev(q, front, front);
	//glm_vec3_rotate(right, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(up, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(front, glm_rad(box->rotation), box->orienation_axis);
	glm_vec3_normalize(right);
	glm_vec3_normalize(up);
	glm_vec3_normalize(front);
	glm_vec3_copy(right, box->X);
	glm_vec3_copy(up, box->Y);
	glm_vec3_copy(front, box->Z);

	//glm_vec3_scale()
	//glm_vec3_add()
	//glm_vec3_scale()
	//glm_vec3_add()
	vec3 temp = GLM_VEC3_ZERO_INIT;
	temp[0] = 1.0f * box->x_size;
	temp[1] = 1.0f * box->y_size;
	temp[2] = 1.0f * box->z_size;

	vec3 max = GLM_VEC3_ZERO_INIT;
	vec3 min = GLM_VEC3_ZERO_INIT;
	vec3 out = GLM_VEC3_ZERO_INIT;

	max[0] = box->x_size;
	max[1] = box->y_size;
	max[2] = box->z_size;

	glm_vec3_copy(max, min);
	glm_vec3_negate(min);

	glm_vec3_copy((vec3){min[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[0]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[1]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[2]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[3]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[4]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[5]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[6]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[7]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);
}

void updateOBBPos(struct OBB *box, vec3 pos) {
	glm_vec3_copy(pos, box->coord);
	vec3 right = {1.0f, 0.0f, 0.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	vec3 front = {0.0f, 0.0f, 1.0f};	//X,Y,Z vectors are default since I can always remember the rotation axis but not original vector pos.
	versor q = GLM_QUAT_IDENTITY_INIT;
	glm_quatv(q, glm_rad(box->rotation), box->orienation_axis);
	//glm_quat_rotatev(q, box->coord, box->coord);
	glm_quat_rotatev(q, right, right);
	glm_quat_rotatev(q, up, up);
	glm_quat_rotatev(q, front, front);
	//glm_vec3_rotate(right, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(up, glm_rad(box->rotation), box->orienation_axis);
	//glm_vec3_rotate(front, glm_rad(box->rotation), box->orienation_axis);
	glm_vec3_normalize(right);
	glm_vec3_normalize(up);
	glm_vec3_normalize(front);
	glm_vec3_copy(right, box->X);
	glm_vec3_copy(up, box->Y);
	glm_vec3_copy(front, box->Z);

	//glm_vec3_scale()
	//glm_vec3_add()
	//glm_vec3_scale()
	//glm_vec3_add()
	vec3 temp = GLM_VEC3_ZERO_INIT;
	temp[0] = 1.0f * box->x_size;
	temp[1] = 1.0f * box->y_size;
	temp[2] = 1.0f * box->z_size;

	vec3 max = GLM_VEC3_ZERO_INIT;
	vec3 min = GLM_VEC3_ZERO_INIT;
	vec3 out = GLM_VEC3_ZERO_INIT;

	max[0] = box->x_size;
	max[1] = box->y_size;
	max[2] = box->z_size;

	glm_vec3_copy(max, min);
	glm_vec3_negate(min);

	glm_vec3_copy((vec3){min[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[0]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[1]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[2]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], min[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[3]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[4]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], min[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[5]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){min[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[6]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);


	glm_vec3_copy((vec3){max[0], max[1], max[2]}, out);
	glm_quat_rotatev(q, out, out);

	glm_vec3_add(box->coord, out, out);
	glm_vec3_copy(out, box->points[7]);
	glm_vec3_copy(GLM_VEC3_ZERO, out);
}

bool OOBvOBBCollisionSATStatic(struct OBB *box1, struct OBB *box2, vec4 result) {	//one or many of these returns true when not supposed to... aka says there's intersect but there isn't
	vec3 axisRes = GLM_VEC3_ZERO_INIT;
	float depth = FLT_MAX;
	if(		//Collision detection in axes of OBB1  x3
			!CheckAxis(box1->points, box2->points, box1->X, axisRes, &depth)
			|| !CheckAxis(box1->points, box2->points, box1->Y, axisRes, &depth)
			|| !CheckAxis(box1->points, box2->points, box1->Z, axisRes, &depth))
	{
		return false;
	}

	if(		//Collision detection in axes of OBB2  x3
			!CheckAxis(box1->points, box2->points, box2->X, axisRes, &depth)
			|| !CheckAxis(box1->points, box2->points, box2->Y, axisRes, &depth)
			|| !CheckAxis(box1->points, box2->points, box2->Z, axisRes, &depth))
	{
		return false;
	}

	if(		//Collision detection in axes of OBB1xOBB2  x9
			!CheckCrossAxis(box1->points, box2->points, box1->X, box2->X, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->X, box2->Y, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->X, box2->Z, axisRes, &depth)

			|| !CheckCrossAxis(box1->points, box2->points, box1->Y, box2->X, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->Y, box2->Y, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->Y, box2->Z, axisRes, &depth)

			|| !CheckCrossAxis(box1->points, box2->points, box1->Z, box2->X, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->Z, box2->Y, axisRes, &depth)
			|| !CheckCrossAxis(box1->points, box2->points, box1->Z, box2->Z, axisRes, &depth))
	{
		return false;
	}
	result[0] = axisRes[0];
	result[1] = axisRes[1];
	result[2] = axisRes[2];
	result[3] = depth;
	return true;
}

bool CheckAxis(vec3 pnts1[], vec3 pnts2[], vec3 axis, vec3 axisRes, float* depth) {
	//printf("checking axis: %f %f %f\n", axis[0], axis[1], axis[2]);
    // Handles the cross product = {0,0,0} case
    if(axis == GLM_VEC3_ZERO)
        return true;

    float aMin = FLT_MAX;
    float aMax = -FLT_MAX;
    float bMin = FLT_MAX;
    float bMax = -FLT_MAX;

    // Define two intervals, a and b. Calculate their min and max values
    //maybe getting garbage values for this look l8r

    //vec3 last_p1s, last_p1m, last_p2s, last_p2m = GLM_VEC3_ZERO_INIT;
    for (int i = 0; i < 8; i++) {
    	//printf("reading p1: %f, %f, %f\n", pnts1[i][0], pnts1[i][1], pnts1[i][2]);
        float aDist = glm_vec3_dot(pnts1[i], axis);
        //printf("aDist %f\n", aDist);
        aMin = ( aDist < aMin ) ? aDist : aMin;
        aMax = ( aDist > aMax ) ? aDist : aMax;
        //printf("reading p2: %f, %f, %f\n", pnts2[i][0], pnts2[i][1], pnts2[i][2]);
        float bDist = glm_vec3_dot(pnts2[i], axis);
        //printf("bDist %f\n", bDist);
        bMin = ( bDist < bMin ) ? bDist : bMin;
        bMax = ( bDist > bMax ) ? bDist : bMax;
    }

    // One-dimensional intersection test between a and b
    //printf("amax: %f, amin: %f, bmax: %f, bmin: %f\n", aMax, aMin, bMax, bMin);
    float longSpan = glm_max(aMax, bMax) - glm_min(aMin, bMin);	//glm_max?
    float sumSpan = aMax - aMin + bMax - bMin;
    if (longSpan <= sumSpan) { 			// Change this to <= if you want the case were they are touching but not overlapping, to count as an intersection
    	float penetration = sumSpan - longSpan;
		if (penetration < *depth) {
			// Always want a normal pointed towards the second box.
			vec3 direction = GLM_VEC3_ZERO_INIT;
			glm_vec3_copy(axis, direction);
			if (aMax > bMax) {
				glm_vec3_negate(direction);
			}

			*depth = penetration;
			glm_vec3_copy(direction, axisRes);
		}
		//printf("overlap found on axis: %f %f %f\n", axis[0], axis[1], axis[2]);
		//printf("returned direction: %f %f %f\n", axisRes[0], axisRes[1], axisRes[2]);
		//printf("long v sum %f, %f\n", longSpan, sumSpan);
		//printf("found penetration of %f\n", penetration);
		//printf("depth of %f\n", *depth);
		//printf("with projection p1s: %f, %f, %f || p2s: %f, %f, %f\n", last_p1s[0], last_p1s[1], last_p1s[2], last_p2s[0], last_p2s[1], last_p2s[2]);
		//printf("with projection p1m: %f, %f, %f || p2m: %f, %f, %f\n", last_p1m[0], last_p1m[1], last_p1m[2], last_p2m[0], last_p2m[1], last_p2m[2]);
    	return true;
    }
    //printf("failed on axis: %f %f %f\n", axis[0], axis[1], axis[2]);
    return false;
}

bool CheckCrossAxis(vec3 pnts1[], vec3 pnts2[], vec3 axis1, vec3 axis2, vec3 axisRes, float* depth) {
	if (fabs(glm_vec3_dot(axis1, axis2)) > 0.99f) {	//The axes are practically parallel
		return true;
	}
	vec3 testAxis = GLM_VEC3_ZERO_INIT;
	glm_vec3_cross(axis1, axis2, testAxis);
	glm_normalize(testAxis);
	return CheckAxis(pnts1, pnts2, testAxis, axisRes, depth);
}



