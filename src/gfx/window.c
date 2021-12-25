#include "window.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cglm/cglm.h>   /* for inline */
//#include <cglm/call.h>   /* for library call (this also includes cglm.h) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global window
struct Window window;
const int TICK_RATE = 66;
unsigned int VAO, EBO;
unsigned int VBO;
//unsigned int texture1, texture2;
unsigned int textures[10];

int Window_init(int wid, int high, char* title) {
	window.wid = wid;
	window.high = high;
	window.title = title;
	window.tick = 0;

	if (!glfwInit()) {
		return 0;
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window.handle = glfwCreateWindow(wid, high, title, NULL, NULL);
	if (!window.handle) {
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window.handle);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    printf("Failed to initialize GLAD!\n");
	    return 0;
	}
	glViewport(0, 0, wid, high);

	//NOTE: cglm uses Column-Major layout so it is sorted as [column][row]:
	// [1, 0, 0, 4]
	// [0, 1, 0, 3]
	// [0, 0, 1, 2]
	// [0, 0, 0, 1]
	//mat4[0][3] == 0, mat4[3][0] == 4
	vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
	printf("%f\n", vec[0]);
	printf("%f\n", vec[1]);
	printf("%f\n", vec[2]);
	printf("%f\n", vec[3]);
	mat4 trans;
	glm_mat4_identity(trans);
	//printf("soem: %f\n",trans[3][3]); //accurate...
	//vec3 temp = {1.0f, 1.0f, 0.0f};
	glm_translate(trans, (vec3){1.0f, 1.0f, 0.0f});

	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("val at (i: %i) (j: %i): %f\n", i, j, trans[i][j]);
		}
	}
	*/
	/* what it should do:
	trans[0][3] = 1.0f;
	trans[1][3] = 1.0f;
	trans[2][3] = 0.0f;
	printf("soem: %f\n",trans[2][3]);
	vec[0] += trans[0][3];
	vec[1] += trans[1][3];
	vec[2] += trans[2][3];
	*/ //but it doesn't?

	glm_mat4_mulv(trans, vec, vec);
	//printf("soem: %f\n",trans[3][3]);
	printf("%f\n", vec[0]);
	printf("%f\n", vec[1]);
	printf("%f\n", vec[2]);
	printf("%f\n", vec[3]);
	glm_mat4_identity(trans);
	//figure way to conv rad to deg
	glm_rotate(trans, (1.571f), (vec3){0.0f, 0.0f, 1.0f});
	glm_scale(trans, (vec3){0.5f, 0.5f, 0.5f});
	//unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)(trans));



	float vertices[] = {	//x, y, z || r, g, b, a || x, y (texture pos)
	     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,		//top right
	     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,		//bottom right
	    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,		//bottom left
	    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f		//top left
	};

	float trigColors[] = {	//r, g, b, a
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f
	};

	int posIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	int colIndices[] = {
		0, 1, 2,   // first triangle
		0, 1, 2    // second triangle
	};

	//unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//gives it data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trigColors), trigColors, GL_STATIC_DRAW);

	// 3. copy our index array in a element buffer for OpenGL to use
	///*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(posIndices), posIndices, GL_STATIC_DRAW);

	//*/


	// 4. then set the vertex attributes pointers
	///*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//*/

	// load and create a texture
	// -------------------------
	///*
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //C:/Users/russa/Desktop/coding_projs/glfwGame/imgs/container.jpg
    //jpg doesn't use alpha so GL_RGB, but png uses a so GL_RGBA...
    unsigned char *data = stbi_load("Resources/Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
	} else {
	    printf("Failed to load texture\n");
	}
	stbi_image_free(data);

	//txter 2
	unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("Resources/Textures/crate.png", &width, &height, &nrChannels, 0);
	if (data) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	//*/
	//generateTexture(&textures[0], "container.jpg");
	//generateTexture(&textures[1], "crate.png");

	//bind both
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	loadShaders(vertexShaderSource, fragmentShaderSource);
	glUseProgram(programID);

	glUniform1i(glGetUniformLocation(programID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(programID, "texture2"), 1);

	unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)(trans));




	//char path[200];
	//getcwd(path, 200);
	//printf("Current working directory: %s\n", path);

	//glfwSwapInterval(1);	//1 for vsync, 0 not
	window_loop();
	return 1;
}	//1 == opened window, 0 == failed

//idk if gflw win needed here but whatevs
void framebuffer_size_callback(GLFWwindow* handle, int width, int height) {
    glViewport(0, 0, width, height);
    window.wid = width;
    window.high = height;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //    glfwSetWindowShouldClose(window, GLFW_TRUE);
    //}
	///*
    if (action == GLFW_PRESS) { //start switch case to see what to do
    	switch (key) {
    		case GLFW_KEY_ESCAPE:
    			glfwSetWindowShouldClose(window, GLFW_TRUE);
    			break;
    		default:
    			break;
    	}
    }
    //*/
}
///*
void generateTexture(unsigned int* txtIndex, const char* file_name) {

	//unsigned int texture;
	glGenTextures(1, txtIndex);
    glBindTexture(GL_TEXTURE_2D, *txtIndex); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //C:/Users/russa/Desktop/coding_projs/glfwGame/imgs/container.jpg
    //jpg doesn't use alpha so GL_RGB, but png uses a so GL_RGBA...
    //char* path[200] = "Resources/Textures/" + file_name;

    unsigned int len = strlen(file_name);
    char* name = (char*)calloc(len, sizeof(char));

    for (int i = len; i > 0; i--) {
    	if (file_name[len] == '.') {
    		break;
    	} else {
    		unsigned int place = strlen(name)-1;
    		name[place] = file_name[i];
    	}
    }
    printf("%s\n", name);
    printf("here?\n");
    if (strcmp(name, "png")) { //if image is png, alpha, else not

    	unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    	if (data) {
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	} else {
    		printf("Failed to load texture\n");
    	}
    	stbi_image_free(data);
    } else {
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
}
//*/

void window_loop() {
	/*Load b4 main loop:*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//for filled

	glBindVertexArray(VAO);


	window.curr_time = glfwGetTime();
	window.prev_time = glfwGetTime();
	window.dt = 0.0;							//delta time
	window.time_passed = 0.0;					//how long program has been running

	while(!glfwWindowShouldClose(window.handle)) {
		//trigColors[5] = (sin(window.curr_time) / 2.0f) + 0.5f;

		//get delta time
		window.curr_time = glfwGetTime();
		//printf("current time: %f\n",window.curr_time);
		//printf("previous time: %f\n",window.prev_time);
		window.dt = window.curr_time - window.prev_time;
		window.prev_time = window.curr_time;
		//printf("delta time: %f\n",window.dt);

		window.tick_float += window.dt * TICK_RATE;
		window.tick = (int)window.tick_float;
		window.time_passed += window.dt;

		glfwSetKeyCallback(window.handle, key_callback);

		//printf("hi\n");
	    //printf("%llu\n", window.tick);
	    //printf("time: %f\n", window.time_passed);
	    if (window.tick >= 3330) {
	    	window.handle = NULL;
	    	glfwDestroyWindow(window.handle);
	    	break;
	    }

	    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    glUseProgram(programID);
	    glBindVertexArray(VAO);

	    //bind data to
	    //glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	    //glEnableVertexAttribArray(0);
	    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	    //glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	    //glEnableVertexAttribArray(1);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	    glfwSwapBuffers(window.handle);
	    glfwPollEvents();
	}

	//glfwDestroyWindow(window.handle);
	glfwTerminate();
}
