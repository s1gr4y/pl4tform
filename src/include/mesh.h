#ifndef MESH_H
#define MESH_H

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h>	//"../../Dependencies/glad/include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h" //<GLFW/glfw3.h>
#include "../../Dependencies/cglm/cglm.h" //<cglm/cglm.h>

#include "../include/shader.h"
#include "../include/OBB.h"
#include "../include/camera.h"

#include "../include/obj_loader.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
	meshType_cube_simple = 0,
	meshType_cube_light = 1,
	meshType_OBJ_simple = 2,
	meshType_OBJ_light = 3,
	meshType_OBJ_all = 4
} meshType;

typedef struct Mesh {
	//  render data
	unsigned int VAO, VBO, EBO;
	unsigned int textures[16];
	// mesh data, all arrs
	float *vertices;
	int *indices;
	unsigned int indexSize;
	//struct Texture *textures;
} Mesh;

struct Texture {
    unsigned int id;
    char* type;
};

typedef struct Object {	//needs orientation (vec3 rotation and axis)
	unsigned int ID;
	meshType type;
	int meshIdx;
	bool one_txture;
	bool lightSrc;
	bool hasCollision;
	vec3 futureVel;
	vec3 velocity;
	void (*velFunc)(float, float, float, vec3);
	vec3 coordinates;
	vec3 scale_dim;
	vec3 collisionHULL_dim;
	vec3 orientation_axis;
	struct OBB box;
	float rotation;	//in deg
} Object;

void initRender();
void addMesh(meshType type, char* path);
void setupMesh(struct Mesh *mesh, float *vertices, unsigned int vertSize, int *indices, unsigned int indexSize);
void setupSimpleMesh(struct Mesh *mesh, float **verts, unsigned int vertSize, int **indices, unsigned int indexSize);
void generateTexture(struct Mesh *mesh, unsigned int txtIndex, const char* file_name);
void generateVAO_VBO_text();
void drawObject(struct Object obj, unsigned int pID, Object* lightObjs, int count, Camera camera);
void drawObjectLight(struct Object obj, unsigned int pID);
void drawObjectSimple(struct Object obj, unsigned int pID);

void ALTdrawObject(struct Object obj, unsigned int pID);

#endif
