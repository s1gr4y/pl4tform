#include "window.h"
#include "shader.h"
#include "../entity/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cglm/cglm.h>   /* for inline */
//#include <cglm/call.h>   /* for library call (this also includes cglm.h) */

// global window
struct Window window;
const int TICK_RATE = 66;
unsigned int VAO, EBO;
unsigned int VBO;
//unsigned int texture1, texture2;
unsigned int textures[10];
int firstMouse = 1;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
vec3 result;
vec3 cameraDirection;

int Window_init(int wid, int high, char* title) {
	window.wid = wid;
	window.high = high;
	window.title = title;
	window.tick = 0;
	window.lastX = wid/2;
	window.lastY = high/2;
	//firstMouse = 1;
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		window.keyboard.keys[i] = 0;
	}

	//init camera
	Camera_init();

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
	mat4 trans;
	glm_mat4_identity(trans);
	//printf("soem: %f\n",trans[3][3]); //accurate...
	//vec3 temp = {1.0f, 1.0f, 0.0f};
	glm_translate(trans, (vec3){1.0f, 1.0f, 0.0f});
	glm_mat4_mulv(trans, vec, vec);
	//printf("soem: %f\n",trans[3][3]);
	//printf("%f\n", vec[0]);
	//printf("%f\n", vec[1]);
	//printf("%f\n", vec[2]);
	//printf("%f\n", vec[3]);
	glm_mat4_identity(trans);
	//figure way to conv rad to deg
	glm_rotate(trans, (1.571f), (vec3){0.0f, 0.0f, 1.0f});
	glm_scale(trans, (vec3){0.5f, 0.5f, 0.5f});
	//unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)(trans));


	/*
	float vertices[] = {	//x, y, z || r, g, b, a || x, y (texture pos)
	     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,		//top right
	     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,		//bottom right
	    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,		//bottom left
	    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f		//top left
	};
	*/
	/*
	float vertices[] = { //x,y,z r,g,b,a, x,y
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};
	*/

	float vertices[] = { //x,y,z | x,y
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
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	};

	/*
	int posIndices[] = {
		0, 1, 2, 3, 4, 5,   // first triangle
		6, 7, 8, 9, 10, 11,  	// second triangle
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};
	*/
	int posIndices[] = {
		0, 1, 2,	//sq 1:
		1, 2, 3,

		4, 5, 6,	//sq 1:
		5, 6, 7,

		8, 9, 10,	//sq 1:
		9, 10, 11,

		12, 13, 14,	//sq 1:
		13, 14, 15,

		16, 17, 18,	//sq 1:
		17, 18, 19,

		20, 21, 22,	//sq 1:
		21, 22, 23
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

	//unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)(trans));





	//char path[200];
	//getcwd(path, 200);
	//printf("Current working directory: %s\n", path);

	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);	//1 for vsync, 0 not
	window_loop();
	return 1;
}	//1 == opened window, 0 == failed

//idk if gflw win needed here but whatevs
void framebuffer_size_callback(GLFWwindow* handle, int width, int height) {
    glViewport(0, 0, width, height);
    window.wid = width;
    window.high = height;
}

static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //    glfwSetWindowShouldClose(window, GLFW_TRUE);
    //}
	///*
	vec3 temp;
	//instead use array of keys and set true when pressed and false until it is released
	if (action < 0) {
		return;
	} else {
		switch (action) {
		        case GLFW_PRESS:
		            window.keyboard.keys[key] = 1;
		            break;
		        case GLFW_RELEASE:
		            window.keyboard.keys[key] = 0;
		            break;
		        default:
		            break;
		}
	}
	/*
	if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
	} else if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_PRESS) { //start switch case to see what to do
    	glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
    	glm_vec3_add(camera.cameraPos, temp, camera.cameraPos);
    } else if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_RELEASE){
	    printf("STOPPED PRESSED!\n");
	}
    */
}

void action_callback() {
	// if key is pressed down do stuff.
	vec3 temp;
	if (window.keyboard.keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
	}
	if (window.keyboard.keys[GLFW_KEY_W]) {
		glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		glm_vec3_add(camera.cameraPos, temp, camera.cameraPos);
	}
	if (window.keyboard.keys[GLFW_KEY_S]) {
		glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		glm_vec3_sub(camera.cameraPos, temp, camera.cameraPos);
	}
	if (window.keyboard.keys[GLFW_KEY_A]) {
		glm_cross(camera.cameraFront, camera.cameraUp, temp);
		glm_normalize(temp);
		glm_vec3_scale(temp, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		glm_vec3_sub(camera.cameraPos, temp, camera.cameraPos);
	}
	if (window.keyboard.keys[GLFW_KEY_D]) {
		glm_cross(camera.cameraFront, camera.cameraUp, temp);
		glm_normalize(temp);
		glm_vec3_scale(temp, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		glm_vec3_add(camera.cameraPos, temp, camera.cameraPos);
	}
}

void mouse_callback(GLFWwindow* handle, double xpos, double ypos) {
	if (firstMouse) {
		window.lastX = xpos;
		window.lastY = ypos;
	    firstMouse = 0;
	}

	float xoffset = xpos - window.lastX;
	float yoffset = window.lastY - ypos; // reversed since y-coordinates range from bottom to top
	window.lastX = xpos;
	window.lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;


	camera.theta = fmod((camera.theta + xoffset), 360);
	//two semicolons means that line of code doesn't run. weird
	//camera.phi = (camera.phi + yoffset);
	camera.phi = fmod((camera.phi + yoffset), 360);
	//camera.phi = 90.0f;
	//camera.phi = fmod((camera.phi + yoffset), 180);;
	/*
	if(camera.phi > 89.9f)
		camera.phi =  89.9f;
	if(camera.phi < -90.0f)
		camera.phi = -90.0f;
	*/

	calc_orientation();
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

	glfwSetKeyCallback(window.handle, key_callback);
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.handle, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);

	window.tick = 0;
	window.curr_time = glfwGetTime();
	window.prev_time = glfwGetTime();
	window.dt = 0.0;							//delta time
	window.time_passed = 0.0;					//how long program has been running

	vec3 cubePositions[] = {
	    { 0.0f,  0.0f,  0.0f},
	    { 2.0f,  5.0f, -15.0f},
	    {-1.5f, -2.2f, -2.5f},
	    {-3.8f, -2.0f, -12.3f},
	    { 2.4f, -0.4f, -3.5f},
	    {-1.7f,  3.0f, -7.5f},
	    { 1.3f, -2.0f, -2.5f},
	    { 1.5f,  2.0f, -2.5f},
	    { 1.5f,  0.2f, -1.5f},
	    {-1.3f,  1.0f, -1.5f}
	};

	glm_vec3_sub(cameraPos, cameraTarget, result);
	glm_vec3_normalize_to(result, cameraDirection);
	vec3 up = {0.0f, 1.0f, 0.0f};
	//use up and overwrite to cameraRight
	vec3 cameraRight;
	glm_cross(up, cameraDirection, cameraRight);
	glm_normalize(cameraRight);
	vec3 cameraUp;
	glm_cross(cameraDirection, cameraRight, cameraUp);
	//mat4 view;
	//glm_look(cameraPos, cameraTarget, up, view);

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

		//glfwSetKeyCallback(window.handle, key_callback);
		//glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);

		//printf("hi\n");
	    //printf("%llu\n", window.tick);
	    //printf("time: %f\n", window.time_passed);
	    if (window.tick >= 3330) {
	    	window.handle = NULL;
	    	glfwDestroyWindow(window.handle);
	    	break;
	    }

	    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	    //rotate
	    /*
	    mat4 trans;
	    glm_mat4_identity(trans);
	    glm_translate(trans, (vec3){0.5f, -0.5f, 0.0f});
	    glm_rotate(trans, sin(glfwGetTime()), (vec3){0.0f, 0.0f, 1.0f});
	    unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)(trans));
	    */
	    // create transformations
		mat4 model;
		//glm_mat4_identity(model);
		mat4 view;
		glm_mat4_identity(view);
		mat4 projection;
		glm_mat4_identity(projection);
		//glm_rotate(model, (float)glfwGetTime() * glm_rad(50.0f), (vec3){0.5f, 1.0f, 0.0f});
		//glm_translate(model, (vec3){0.0f, -0.5f, 0.0f});
		//glm_translate(view, (vec3){0.0f, 0.0f, 0.0f});
		//printf("%f", 2*cos(glm_rad(window.tick-90)));
		//glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

		//glm_rotate(view, glm_rad(window.tick), (vec3){0.0f, 1.0f, 0.0f});
		glm_perspective(glm_rad(90.0f), (float)window.wid / (float)window.high, 0.1f, 100.0f, projection);
		// retrieve the matrix uniform locations
		//glm_rotate(projection, glm_rad(window.tick), (vec3){0.0f, 1.0f, 0.0f});

		//float radius = 6.0f;
		//float camX = sin(glm_rad(window.tick)) * radius;
		//float camZ = cos(glm_rad(window.tick)) * radius;
		//printf("%i\n", window.tick);
		//mat4 view;
		//calc_orientation();

		vec3 tempVec;
		glm_vec3_zero(tempVec);
		glm_vec3_copy(camera.direction, camera.cameraFront);
		glm_vec3_add(camera.cameraPos, camera.cameraFront, tempVec);
		//tmp vec lets us look straight
		//glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		//update everything b4 changing cam
		//glm_vec3_add(camera.cameraPos, camera.cameraFront, camera.cameraFront);
		//printf("cam theta: %f\n", sin(glm_rad(camera.theta)));
		printf("theta: %f\n", camera.theta);
		printf("phi: %f\n", camera.phi);
		float temp_phi = 0.0f;
		float temp_theta = 0.0f;
		if (camera.phi < 0.0f) {
			temp_phi = (camera.phi + 360.0f);
		}
		if (camera.theta < 0.0f) {
			temp_theta = (camera.theta + 360.0f);
		}



		/*
		if (temp_phi >= 270.0f) { //camera.phi <= -90.0f
			glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else if (temp_phi >= 90.0f) {
			glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else {
			//set upvector to be the based on player theta with x and z ignore y
			glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		}
		*/
		//all cases and has correct up vector.
		if (camera.phi <= -270.0f) {
			glm_vec3_copy((vec3){-cos(glm_rad(camera.theta)), 0.0f, -sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else if (camera.phi <= -180.0f) {
			glm_vec3_copy((vec3){0.0f, -1.0f, 0.0f}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else if (camera.phi <= -90.0f) { //camera.phi <= -90.0f
			glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else if (camera.phi >= 270.0f) {
			glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		}else if (camera.phi >= 180.0f) {
			glm_vec3_copy((vec3){0.0f, -1.0f, 0.0f}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else if (camera.phi >= 90.0f) {
			glm_vec3_copy((vec3){-cos(glm_rad(camera.theta)), 0.0f, -sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else { //between 90 and -90
			//set upvector to be the based on player theta with x and z ignore y
			glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		}


		/*
		if (camera.phi <= -90.0f) { //camera.phi <= -90.0f
			glm_vec3_copy((vec3){cos(glm_rad(camera.theta)), 0.0f, sin(glm_rad(camera.theta))}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		} else {
			//set upvector to be the based on player theta with x and z ignore y
			glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.cameraUp);
			glm_lookat(camera.cameraPos, tempVec, camera.cameraUp, camera.lookAt_mat);
		}
		*/


		//unsigned int modelLoc = glGetUniformLocation(programID, "model");
		unsigned int viewLoc  = glGetUniformLocation(programID, "view");
		unsigned int projLoc  = glGetUniformLocation(programID, "projection");
		// pass them to the shaders (3 different ways)
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)camera.lookAt_mat);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		for(unsigned int i = 0; i < 10; i++) {
			glm_mat4_identity(model);
		    glm_translate(model, cubePositions[i]);
		    float angle = 20.0f * i;
		    glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
		    if (i == 0) {
		    	glm_rotate(model, (float)glfwGetTime() * glm_rad(50.0f), (vec3){0.5f, 1.0f, 0.0f});
		    }
		    unsigned int modelLoc = glGetUniformLocation(programID, "model");
		    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
		    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);	//for EBO
		    //glDrawArrays(GL_TRIANGLES, 0, 36);	//to use VAO
		}

		glfwPollEvents();
		action_callback();
		glfwSwapBuffers(window.handle);
	}

	//glfwDestroyWindow(window.handle);
	glfwTerminate();
}
