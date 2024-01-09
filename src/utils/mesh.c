#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/stb_image.h"

const unsigned int SHADOW_WIDTH = 1920;
const unsigned int SHADOW_HEIGHT = 1080;

unsigned int depthMapFBO;	//universal for the "sun"
unsigned int depthMap;

unsigned int quadVAO;
unsigned int quadVBO;

float quadVertices[] = {
    // positions        // texture Coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

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
	stbi_set_flip_vertically_on_load(true);
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
	if (mesh->indexSize > 0) {	// may not need this anymore
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Mesh mesh = mList[obj.meshIdx];
	mat4 model;
	mat4 tmpMatNorm;
	mat3 matrixNormal;
	int i = 0;
	for (i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh.textures[i]);
	}
	//printf("i %d\n", i);
	// following 2 lines not needed
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, depthMap);	// shadow map for the sun

	// this was the line of code not letting it work
	glUniform1i(glGetUniformLocation(pID, "shadowMap"), i);

	float near_plane = 1.0f, far_plane = 7.5f;
    mat4 lightProjection;
    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, lightProjection);
    mat4 lightView;
    glm_lookat(  lightObjs->coordinates, // light position
                 (vec3){ 0.0f, 0.0f, 0.0f}, 
                 (vec3){ 0.0f, 1.0f, 0.0f}, lightView);
    mat4 lightSpaceMatrix;
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    glUniformMatrix4fv(glGetUniformLocation(pID, "lightSpaceMatrix"), 1, GL_FALSE, (float*)(lightSpaceMatrix));


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
	
	glm_mat4_copy(model, tmpMatNorm);
	glm_mat4_inv(tmpMatNorm, tmpMatNorm);			//not sure if inverse and transpose are necessary but will keep copy
	glm_mat4_transpose_to(tmpMatNorm, tmpMatNorm);
	
	glm_mat4_pick3(tmpMatNorm, matrixNormal);
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
	glBindVertexArray(0);
}

void drawObjectSimple(struct Object obj, struct Mesh* mList, unsigned int pID) {	//debateable if needs to pass in by pointer not copy, but doesn't matter
	// draw model
	Mesh mesh = mList[obj.meshIdx];
	mat4 model;
	vec3 color = GLM_VEC3_ZERO_INIT;
	color[0] = 0.3f; color[1] = 0.5f; color[0] = 0.7f;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mList[obj.meshIdx].textures[1]);	// may not need to point to specific one
	
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
	if (mesh.indexSize <= 0) {
		//printf("obj id %d\n", obj.ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	} else {
		glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, 0);
		//printf("%d\n", mesh.indexSize);
	}
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




void setupQuadVAO_VBO() {
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void DrawQuad(unsigned int prgID) {
	glUseProgram(prgID);
    glBindVertexArray(quadVAO);
	float near_plane = 1.0f, far_plane = 7.5f;
	glUniform1f(glGetUniformLocation(prgID, "near_plane"), near_plane);
    glUniform1f(glGetUniformLocation(prgID, "far_plane"), far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

	glUniform1i(glGetUniformLocation(prgID, "depthMap"), 0);	//0 since we use first one activated

        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void generateDepthFBO() {
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderDepthMap(Object lightSrc, unsigned int prgID) {
    glUseProgram(prgID);

    // generating perspective
    float near_plane = 1.0f, far_plane = 7.5f;
    mat4 lightProjection;
    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, lightProjection);
    mat4 lightView;
    glm_lookat(  lightSrc.coordinates, // light position
                 (vec3){ 0.0f, 0.0f, 0.0f}, 
                 (vec3){ 0.0f, 1.0f, 0.0f}, lightView);
    mat4 lightSpaceMatrix;
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    glUniformMatrix4fv(glGetUniformLocation(prgID, "lightSpaceMatrix"), 1, GL_FALSE, (float*)(lightSpaceMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // you render all obj here from light pov
    //RenderScene(programIDDepthShader);    //dne yet

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

