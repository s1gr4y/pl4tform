#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/stb_image.h"

void initRender() {	//may not need anymore
	//glActiveTexture(GL_TEXTURE0 + 1); //say we have 16 txtures.
	/*
	
	//setupMesh(&(meshList[2]), vertices, sizeof(vertices), NULL, 0);
	glGenVertexArrays(1, &(meshList[2].VAO));
	glGenVertexArrays(1, &(meshList[2].VBO));
	glBindVertexArray((meshList[2].VAO));
	glBindBuffer(GL_ARRAY_BUFFER, meshList[2].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	*/

	//setupSimpleMesh(&(meshList[3]), &data, data_len, &indices, indices_len);
	//setupSimpleMesh(&(meshList[3]), square_vertices, sizeof(square_vertices), square_posIndices, sizeof(square_posIndices));
	//setupSimpleMesh(&(meshList[3]), square_vertices, sizeof(square_vertices), square_posIndices, sizeof(square_posIndices));
}

void setupSimpleMesh(struct Mesh *mesh, float *verts, unsigned int vertSize, int *indices, unsigned int indexSize) {
	//unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(mesh->VAO);

	if (verts != NULL) {
		mesh->vertices = verts;
	} else {
		mesh->vertices = NULL;
	}
	if (indices != NULL) {
		mesh->indices = indices;
	} else {
		mesh->indices = NULL;
	}
	mesh->indexSize = indexSize;

	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//gives it data
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize*sizeof(float), mesh->vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trigColors), trigColors, GL_STATIC_DRAW);

	//*/

	// 4. then set the vertex attributes pointers
	///*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//*/
	// load and create a texture
	// -------------------------
	//stbi_set_flip_vertically_on_load(true);
	//glEnable(GL_TEXTURE_2D);
	// should pass in path to texture for given object but we will worry about that later
	generateTexture(mesh, 0, "Resources/Textures/container.jpg");	//need to figure out why texture no work T_T; 6/17 (fixed a while ago (and forgot) but was an annoying bug, keeping comment)
	generateTexture(mesh, 1, "Resources/Textures/crate.png");
	generateTexture(mesh, 2, "Resources/Textures/dev_64.png");
	glBindVertexArray(0);
}

void setupMesh(struct Mesh *mesh, float **vertices, unsigned int vertSize, int **indices, unsigned int indexSize) {
	//unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(mesh->VAO);

	if (vertices != NULL) {
		mesh->vertices = *vertices;
	} else {
		mesh->vertices = NULL;
	}
	if (indices != NULL) {
		mesh->indices = *indices;
	} else {
		mesh->indices = NULL;
	}
	mesh->indexSize = indexSize;
	//for (int i = 0; i < vertSize)

	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//gives it data
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize*sizeof(float), mesh->vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trigColors), trigColors, GL_STATIC_DRAW);

	// 3. copy our index array in a element buffer for OpenGL to use
	///*
	if (mesh->indices != NULL) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(int), mesh->indices, GL_STATIC_DRAW);
	}

	//*/


	// 4. then set the vertex attributes pointers
	///*
	if (mesh->indexSize > 0) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);
	} else {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	//*/

	// load and create a texture
	// -------------------------
	//stbi_set_flip_vertically_on_load(true);
	//glEnable(GL_TEXTURE_2D);
	// should pass in path to texture for given object but we will worry about that later
	generateTexture(mesh, 0, "Resources/Textures/container.jpg");	//need to figure out why texture no work T_T; 6/17 (fixed a while ago (and forgot) but was an annoying bug, keeping comment)
	generateTexture(mesh, 1, "Resources/Textures/crate.png");
	generateTexture(mesh, 2, "Resources/Textures/dev_64.png");
	//glUniform1f(glGetUniformLocation(programID, "mixer"), 0.5f);
	glBindVertexArray(0);
}

void generateTexture(struct Mesh *mesh, unsigned int txtIndex, const char* file_name) {
	//glActiveTexture(GL_TEXTURE0 + txtIndex); //0 - 16, cycle influences it.
	//unsigned int texture;
	glGenTextures(1, &mesh->textures[txtIndex]);
    glBindTexture(GL_TEXTURE_2D, mesh->textures[txtIndex]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    //set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
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
    		//printf("h3h3\n");
    		//printf("%c\n", file_name[i]);
    		name[index] = file_name[i];
    		index++;
    	}
    }
    //printf("%c\n", name[0]);
    //printf("%s - end?\n", name);
    if (strcmp(name, "png") == 0) { //if image is png, alpha, else not
    	//printf("here1?\n");
    	unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    	if (data) {
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	} else {
    		printf("Failed to load texture\n");
    	}
    	stbi_image_free(data);
    } else {
    	//printf("here2?\n");
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

    //printf("almost\n");
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

void drawObject(struct Object obj, struct Mesh* mList, unsigned int pID, Object* lightObjs, int count, Camera camera) {	//debateable if needs to pass in by pointer not copy, but doesn't matter
	// draw model
	Mesh mesh = mList[obj.meshIdx];
	mat4 model;
	mat4 tmpMatNorm;
	mat3 matrixNormal;
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh.textures[i]);
	}
	glBindVertexArray(mesh.VAO);
	glm_mat4_identity(model);
	glm_translate(model, obj.coordinates);
	glm_rotate(model, glm_rad(obj.rotation), obj.orientation_axis);
	glm_scale(model, obj.scale_dim);

	if (count >= 64) {
		count = 64;
	}
	
	glUniform3f(glGetUniformLocation(pID, "viewPos"), camera.cameraPos[0], camera.cameraPos[1], camera.cameraPos[2]);
	glUniform1f(glGetUniformLocation(pID, "material.shininess"), 8.0f);
	glUniform1i(glGetUniformLocation(pID, "LIGHT_CAP"), count);
	
	glUniform3f(glGetUniformLocation(pID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(pID, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(pID, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(pID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        
        
    //char line[] = pointLights[0].position
	//fix later lazy
	for (int i = 0; i < count; i++) {
		char buffer[64];
		vec3 tmp = GLM_VEC3_ZERO_INIT;
		glm_vec3_copy(lightObjs[0].coordinates, tmp);

		sprintf(buffer, "pointLights[%i].position", i);
		glUniform3f(glGetUniformLocation(pID, buffer), tmp[0], tmp[1], tmp[2]);

		sprintf(buffer, "pointLights[%i].ambient", i);
		glUniform3f(glGetUniformLocation(pID, buffer), 0.05f, 0.05f, 0.05f);
		
		sprintf(buffer, "pointLights[%i].diffuse", i);
		glUniform3f(glGetUniformLocation(pID, buffer), 0.8f, 0.8f, 0.8f);
		
		sprintf(buffer, "pointLights[%i].specular", i);
		glUniform3f(glGetUniformLocation(pID, buffer), 1.0f, 1.0f, 1.0f);
		
		sprintf(buffer, "pointLights[%i].constant", i);
		glUniform1f(glGetUniformLocation(pID, buffer), 1.0f);

		sprintf(buffer, "pointLights[%i].linear", i);
		glUniform1f(glGetUniformLocation(pID, buffer), 0.09f);

		sprintf(buffer, "pointLights[%i].qaudratic", i);
		glUniform1f(glGetUniformLocation(pID, buffer), 0.032f);
	}
        
        glUniform3f(glGetUniformLocation(pID, "spotLight.position"), camera.cameraPos[0], camera.cameraPos[1], camera.cameraPos[2]);
        glUniform3f(glGetUniformLocation(pID, "spotLight.direction"), camera.cameraFront[0], camera.cameraFront[1], camera.cameraFront[2]);
        glUniform3f(glGetUniformLocation(pID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(pID, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(pID, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(pID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(pID, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(pID, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(pID, "spotLight.cutOff"), cosf(glm_rad(12.5f)));
        glUniform1f(glGetUniformLocation(pID, "spotLight.outerCutOff"), cosf(glm_rad(15.0f)));  
	
	if (obj.type == meshType_OBJ_simple || obj.type == meshType_OBJ_light) {	// aka is obj textured or not
		glUniform1i(glGetUniformLocation(pID, "using_color"), 1);	// setting to 1 so we say we are using only color
		vec3 color = GLM_VEC3_ZERO_INIT;
		color[0] = 0.3f; color[1] = 0.5f; color[0] = 0.7f;			//setting arbitrary color
		unsigned int colorLoc = glGetUniformLocation(pID, "color");
		glUniform3fv(colorLoc, 1, (float*)color);
	} else {
		glUniform1i(glGetUniformLocation(pID, "using_color"), 0);	// set to 0, not using color
		if (obj.one_txture == true) {
			//printf("setting to dev texture\n");
			glUniform1i(glGetUniformLocation(pID, "material.texture1"), 2);
			glUniform1i(glGetUniformLocation(pID, "material.texture2"), 2);
			glUniform1f(glGetUniformLocation(pID, "mixer"), 1.0f);
		} else {
			glUniform1i(glGetUniformLocation(pID, "material.texture1"), 1);
			glUniform1i(glGetUniformLocation(pID, "material.texture2"), 1);
			glUniform1f(glGetUniformLocation(pID, "mixer"), 0.5f);
		}
	}
	
	unsigned int matrixNormalLoc = glGetUniformLocation(pID, "matrixNormal");
	
	//glm_mat4_copy(model, tmpMatNorm);
	glm_mat4_pick3(model, matrixNormal);
	//glm_mat4_inv(model, tmpMatNorm);			//not sure if inverse and transpose are necessary but will keep copy
	//glm_mat4_transpose_to(tmpMatNorm, tmpMatNorm);
	/*
	int j = -1;
	for (int i = 0; i < 9; i++) {	// loop to convert column dominant mat4 to mat3
		if (i % 3 == 0) {
			j += 1;
		}
		matrixNormal[j][i%3] = tmpMatNorm[j][i%3];
	}
	*/
	//glm_mat3_copy((float (*)[3])tmpMatNorm, matrixNormal);	// copy the first 3 rows and cols from 4x4 mat to get scalar part along diaganal and not transform
	glUniformMatrix3fv(matrixNormalLoc, 1, GL_FALSE, (float*)matrixNormal);
	
	unsigned int modelLoc = glGetUniformLocation(pID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	if (mesh.indexSize <= 0) {
		//printf("obj id %d\n", obj.ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	} else {
		glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
		//printf("%d\n", mesh.indexSize);
	}
	//glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawObjectSimple(struct Object obj, struct Mesh* mList, unsigned int pID) {	//debateable if needs to pass in by pointer not copy, but doesn't matter
	// draw model
	Mesh mesh = mList[obj.meshIdx];
	mat4 model;
	vec3 color = GLM_VEC3_ZERO_INIT;
	color[0] = 0.3f; color[1] = 0.5f; color[0] = 0.7f;
	
	glBindVertexArray(mesh.VAO);
	glm_mat4_identity(model);
	glm_translate(model, obj.coordinates);
	glm_rotate(model, glm_rad(obj.rotation), obj.orientation_axis);
	glm_scale(model, obj.scale_dim);

	unsigned int colorLoc = glGetUniformLocation(pID, "color");
	glUniform3fv(colorLoc, 1, (float*)color);
	
	unsigned int modelLoc = glGetUniformLocation(pID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
	//printf("%d\n", mesh.indexSize);
	glBindVertexArray(0);
}

void drawObjectLight(struct Object obj, struct Mesh* mList, unsigned int pID) {	//debateable if needs to pass in by pointer not copy, but doesn't matter
	Mesh mesh = mList[obj.meshIdx];
	mat4 model;
	mat4 tmpMatNorm;
	mat3 matrixNormal;
	
	glBindVertexArray(mesh.VAO);
	glm_mat4_identity(model);
	glm_translate(model, obj.coordinates);
	glm_rotate(model, glm_rad(obj.rotation), obj.orientation_axis);
	glm_scale(model, obj.scale_dim);
	
	//unsigned int matrixNormalLoc = glGetUniformLocation(pID, "matrixNormal");
	
	//glm_mat4_inv(model, tmpMatNorm);
	//glm_mat4_transpose_to(tmpMatNorm, tmpMatNorm);
	//glm_mat3_copy((float (*)[3])tmpMatNorm, matrixNormal);	// copy the first 3 rows and cols from 4x4 mat to get scalar part along diaganal and not transform
	//glUniformMatrix3fv(matrixNormalLoc, 1, GL_FALSE, (float*)matrixNormal);
	
	
	unsigned int modelLoc = glGetUniformLocation(pID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

