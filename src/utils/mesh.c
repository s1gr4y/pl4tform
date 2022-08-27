#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/stb_image.h"

Mesh meshList[2];	//only tri and cubes

void initRender() {
	//glActiveTexture(GL_TEXTURE0 + 1); //say we have 16 txtures.
	float square_vertices[] = { //x,y,z | x,y
		//front / back faces
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		//left / right faces
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 //bottom / top faces
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};
	int square_posIndices[] = {
		0, 1, 2,	//sq 1:
		1, 2, 3,

		4, 5, 6,	//sq 2:
		5, 6, 7,

		8, 9, 10,	//sq 3:
		9, 10, 11,

		12, 13, 14,	//sq 4:
		13, 14, 15,

		16, 17, 18,	//sq 5:
		17, 18, 19,

		20, 21, 22,	//sq 6:
		21, 22, 23
	};

	float triangle_vertices[] = { //x,y,z | x,y
		//front / back faces
		 0.5f, -0.5f,  -0.5f,  1.0f, 0.0f,	//bottom right | -z
		-0.5f, -0.5f,  -0.5f,  0.0f, 0.0f,	//bottom left
		 0.0f,  0.5f,  0.0f,  0.5f, 1.0f,	//top mid.

		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	//bottom right | +z
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	//bottom left

		 //0.0f,  0.5f,  0.0f,  0.5f, 1.0f,	//top mid.	//redundant
		 0.5f, -0.5f,  -0.5f,  1.0f, 0.0f,	//bottom right | -z
		-0.5f, -0.5f,  -0.5f,  0.0f, 0.0f,	//bottom left
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,	//bottom right | +z
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f	//bottom left
	};

	int triangle_posIndices[] = {
		0, 1, 2,	//tri 1: back

		0, 3, 2,	//tri 2: right

		1, 4, 2,	//tri 3: left

		3, 4, 2,	//tri 4: front

		5, 6, 7,	//bottom piece 1:
		6, 8, 7		//bottom piece 2:
	};

	//0 == sq, 1 == triag
	setupMesh(&(meshList[0]), square_vertices, sizeof(square_vertices), square_posIndices, sizeof(square_posIndices));
	setupMesh(&(meshList[1]), triangle_vertices, sizeof(triangle_vertices), triangle_posIndices, sizeof(triangle_posIndices));
}

void setupMesh(struct Mesh *mesh, float *vertices, unsigned int vertSize, int *indices, unsigned int indexSize) {
	//unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(mesh->VAO);

	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//gives it data
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trigColors), trigColors, GL_STATIC_DRAW);

	// 3. copy our index array in a element buffer for OpenGL to use
	///*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

	//*/


	// 4. then set the vertex attributes pointers
	///*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//*/

	// load and create a texture
	// -------------------------
	//stbi_set_flip_vertically_on_load(true);
	//glEnable(GL_TEXTURE_2D);
	generateTexture(mesh, 0, "Resources/Textures/container.jpg");	//need to figure out why texture no work T_T; 6/17
	generateTexture(mesh, 1, "Resources/Textures/crate.png");
	generateTexture(mesh, 2, "Resources/Textures/dev_64.png");
	//glUniform1f(glGetUniformLocation(programID, "mixer"), 0.5f);
	mesh->indexSize = indexSize;
}

void generateTexture(struct Mesh *mesh, unsigned int txtIndex, const char* file_name) {
	//glActiveTexture(GL_TEXTURE0 + txtIndex); //0 - 16, cycle influences it.
	//unsigned int texture;
	glGenTextures(1, &mesh->txture[txtIndex]);
    glBindTexture(GL_TEXTURE_2D, mesh->txture[txtIndex]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    unsigned int len = strlen(file_name);
    char* name = (char*)calloc(len, sizeof(char));
    bool sw = false;
    int index = 0;
    for (int i = 0; i < len; i++) {
    	//printf("%c\n", file_name[i]);
    	if (file_name[i] == '.') {
    		sw = true;
    		continue;
    	}
    	if (sw == true) {
    		printf("h3h3\n");
    		printf("%c\n", file_name[i]);
    		name[index] = file_name[i];
    		index++;
    	}
    }
    printf("%c\n", name[0]);
    printf("%s - end?\n", name);
    if (strcmp(name, "png") == 0) { //if image is png, alpha, else not
    	printf("here1?\n");
    	unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    	if (data) {
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	} else {
    		printf("Failed to load texture\n");
    	}
    	stbi_image_free(data);
    } else {
    	printf("here2?\n");
    	unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    	if (data) {
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	} else {
    		printf("Failed to load texture\n");
    	}
    	stbi_image_free(data);
    }
    free(name);

    printf("almost\n");
    //glActiveTexture(GL_TEXTURE0 + txtIndex);

    /*	//loads this into our shader, unnecessary for now.
    if (txtIndex == 1) {	//lazy sol to say txt 0 str.
    	glUniform1i(glGetUniformLocation(programID, "texture1"), txtIndex);
    } else {
    	glUniform1i(glGetUniformLocation(programID, "texture0"), txtIndex);
    }
    */
    //glBindTexture(GL_TEXTURE_2D, 0);
}

void drawObject(struct Object obj, unsigned int pID) {
	// draw model
	Mesh mesh = meshList[obj.type];
	mat4 model;
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh.txture[i]);
	}
	glBindVertexArray(mesh.VAO);
	glm_mat4_identity(model);
	glm_translate(model, obj.coordinates);
	glm_rotate(model, glm_rad(obj.rotation), obj.orientation_axis);
	glm_scale(model, obj.scale_dim);
	if (obj.one_txture == true) {
		glUniform1i(glGetUniformLocation(pID, "texture1"), 2);
		glUniform1i(glGetUniformLocation(pID, "texture2"), 2);
		glUniform1f(glGetUniformLocation(pID, "mixer"), 1.0f);
	} else {
		glUniform1i(glGetUniformLocation(pID, "texture1"), 1);
		glUniform1i(glGetUniformLocation(pID, "texture2"), 1);
		glUniform1f(glGetUniformLocation(pID, "mixer"), 0.5f);
	}
	unsigned int modelLoc = glGetUniformLocation(pID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}
