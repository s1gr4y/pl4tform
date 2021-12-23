#include "window.h"

#include <stdio.h>
#include <stdlib.h>

// global window
struct Window window;
const int TICK_RATE = 66;

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

	float vertices[] = {	//x, y, z
	     0.5f,  0.5f, 0.0f,  // top right
	     0.5f, -0.5f, 0.0f,  // bottom right
	    -0.5f, -0.5f, 0.0f,  // bottom left
	    -0.5f,  0.5f, 0.0f   // top left
	};

	float trigColors[] = {	//r, g, b, a
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	int posIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	int colIndices[] = {
		0, 1, 2,   // first triangle
		0, 1, 2    // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(posIndices), posIndices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	glfwSwapInterval(0);	//1 for vsync, 0 not
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void window_loop() {
	/*Load b4 main loop:*/
	window.curr_time = glfwGetTime();
	window.prev_time = glfwGetTime();
	window.dt = 0.0;							//delta time
	window.time_passed = 0.0;					//how long program has been running

	while(!glfwWindowShouldClose(window.handle)) {

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
	    if (window.tick >= 330) {
	    	window.handle = NULL;
	    	glfwDestroyWindow(window.handle);
	    	break;
	    }

	    glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    glfwSwapBuffers(window.handle);
	    glfwPollEvents();
	}

	//glfwDestroyWindow(window.handle);
	glfwTerminate();
}
