#ifndef MESH_H
#define MESH_H

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h>	//"../../Dependencies/glad/include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h" //<GLFW/glfw3.h>
#include "../../Dependencies/cglm/cglm.h" //<cglm/cglm.h>

#include "../include/shader.h"
#include "../include/OBB.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
	meshType_cube = 0,
	meshType_triangle = 1
} meshType;

typedef struct Mesh {
	//  render data
	unsigned int VAO, VBO, EBO;
	unsigned int txture[3];
	// mesh data, all arrs
	//struct Vertex *vertices;
	float *vertices;
	float *indices;
	float *textures;
	int indexSize;
	//struct Texture *textures;
} Mesh;

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    char* type;
};

typedef struct Object {	//needs orientation (vec3 rotation and axis)
	unsigned int ID;
	meshType type;
	bool one_txture;
	vec3 futureVel;
	vec3 velocity;
	void (*velFunc)(float, float, float, vec3);
	vec3 coordinates;
	vec3 scale_dim;
	vec3 orientation_axis;
	struct OBB box;
	float rotation;	//in deg
} Object;

void initRender();
void setupMesh(struct Mesh *mesh, float *vertices, unsigned int vertSize, int *indices, unsigned int indexSize);
void generateTexture(struct Mesh *mesh, unsigned int txtIndex, const char* file_name);
void generateVAO_VBO_text();
void drawObject(struct Object obj, unsigned int pID);

extern Mesh meshList[2];

#endif
