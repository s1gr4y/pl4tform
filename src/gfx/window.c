#include "window.h"
//#include "shader.h"
#include "../entity/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cglm/cglm.h>   /* for inline */
//#include <cglm/call.h>   /* for library call (this also includes cglm.h) */

// global window
struct Window window;
int firstMouse = 1;

vec3 cameraPos = {0.0f, 0.0f, 0.0f};
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

	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(0);	//1 for vsync, 0 not
	int temp_high, temp_wid = 0;
	get_resolution(&temp_wid, &temp_high);
	glfwSetWindowPos(window.handle, (temp_wid-window.wid)/2, (temp_high-window.high)/2); //set the window position to mid

	initRender();
	initWorld();
	loadShaders(vertexShaderSource, fragmentShaderSource);
	glUseProgram(programID);
	window_loop();
	return 1;
}	//1 == opened window, 0 == failed

//idk if gflw win needed here but whatevs
void framebuffer_size_callback(GLFWwindow* handle, int width, int height) {
    glViewport(0, 0, width, height);
    window.wid = width;
    window.high = height;
}

void get_resolution(int *width, int *high) {
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //this is current monitor resolution.
    *width = mode->width;
    *high = mode->height;
}

static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
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
}


//for quaternions here...
void action_callback() {
	// if key is pressed down do stuff.
	if (window.keyboard.keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
	}
	if (window.keyboard.keys[GLFW_KEY_W]) {
		vec3 temp;
		glm_vec3_zero(temp);
		//glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		temp[0] = cos(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		temp[2] = sin(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		//glm_vec3_add(player.velocity, temp, player.velocity);
		if (player.in_air == false) {
			glm_vec3_add(player.velFoward, temp, player.velFoward);
		} else {
			glm_vec3_scale(temp, 0.2f, temp);
			glm_vec3_add(player.velFoward, temp, player.velFoward);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_S]) {
		//glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		vec3 temp;
		glm_vec3_zero(temp);
		temp[0] = cos(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		temp[2] = sin(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		//glm_vec3_sub(player.velocity, temp, player.velocity);
		if (player.in_air == false) {
			glm_vec3_sub(player.velBack, temp, player.velBack);
		} else {
			glm_vec3_scale(temp, 0.2f, temp);
			glm_vec3_sub(player.velBack, temp, player.velBack);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_A]) {
		vec3 temp;
		glm_vec3_zero(temp);
		glm_cross(player.camera.cameraFront, player.camera.cameraUp, temp);
		//glm_vec3_copy({cos(fmod((camera.theta + 90.0f), 360)), sin(fmod((camera.theta + 90.0f), 360)), 0.0f}, temp);	//+90 since we vec3_sub.
		glm_normalize(temp);
		glm_vec3_scale(temp, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		//glm_vec3_sub(player.velocity, temp, player.velocity);
		if (player.in_air == false) {
			glm_vec3_sub(player.velLeft, temp, player.velLeft);
		} else {
			glm_vec3_scale(temp, 0.2f, temp);
			glm_vec3_sub(player.velLeft, temp, player.velLeft);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_D]) {
		vec3 temp;
		glm_vec3_zero(temp);
		glm_cross(player.camera.cameraFront, player.camera.cameraUp, temp);
		//glm_vec3_copy({cos(fmod((camera.theta + 90.0f), 360)), sin(fmod((camera.theta + 90.0f), 360)), 0.0f}, temp);
		glm_normalize(temp);
		glm_vec3_scale(temp, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		//glm_vec3_add(player.velocity, temp, player.velocity);
		if (player.in_air == false) {
			glm_vec3_add(player.velRight, temp, player.velRight);
		} else {
			glm_vec3_scale(temp, 0.2f, temp);
			glm_vec3_add(player.velRight, temp, player.velRight);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_SPACE] && player.in_air == false && player.jumping == false) {	// && player.in_air == true?	//so we don't inf jump but lol y not.
		vec3 temp;
		glm_vec3_zero(temp);
		temp[1] = CAM_SPEED * 5;	//3.5
		//glm_vec3_add(player.velocity, temp, player.velocity);
		glm_vec3_copy(temp, player.velUp);
		//player.in_air = true;
		player.jumping = true;
		//printf("AAAAAAAAAAAAAAAAA: %f, %f, %f\n", player.velUp[0], player.velUp[1], player.velUp[2]);
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

	const float sensitivity = 0.02f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;


	player.camera.theta = fmod((player.camera.theta + xoffset), 360);
	//two semicolons means that line of code doesn't run. weird
	//camera.phi = (camera.phi + yoffset);
	player.camera.phi = fmod((player.camera.phi + yoffset), 360);
	//camera.phi = 90.0f;
	//camera.phi = fmod((camera.phi + yoffset), 180);;
	///*
	if(player.camera.phi >= 89.9f)
		player.camera.phi =  89.9f;
	if(player.camera.phi < -89.9f)
		player.camera.phi = -89.9f;
	//*/

	calc_orientation(&player.camera);
}

void window_loop() {
	/*Load b4 main loop:*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//for filled

	glfwSetKeyCallback(window.handle, key_callback);
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.handle, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	//glBindVertexArray(VAO);	//which VAO we get data from.

	window.tick = 0;
	window.curr_time = glfwGetTime();
	window.prev_time = glfwGetTime();
	window.dt = 0.0;							//delta time
	window.time_passed = 0.0;					//how long program has been running

	mat4 projection;
	glm_mat4_identity(projection);
	glm_perspective(glm_rad(90.0f), (float)window.wid / (float)window.high, 0.1f, 100.0f, projection);

	while(!glfwWindowShouldClose(window.handle)) {

		//get delta time
		window.curr_time = glfwGetTime();
		//printf("current time: %f\n",window.curr_time);
		//printf("previous time: %f\n",window.prev_time);
		window.dt = window.curr_time - window.prev_time;
		window.prev_time = window.curr_time;
		printf("delta time: %f\n",window.dt);

		window.tick_float += window.dt * TICK_RATE;
		window.tick = (int)window.tick_float;
		window.time_passed += window.dt;

	    //printf("time: %f\n", window.time_passed);
	    if (window.tick >= 999999) { //3330
	    	window.handle = NULL;
	    	glfwDestroyWindow(window.handle);
	    	break;
	    }

	    glClearColor(0.36f, 0.66f, 0.86f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    //glUseProgram(programID);
	    ComputePositionPlayer(&player, window.dt * TICK_RATE);

	    //player.coords[0] += window.dt * TICK_RATE * player.velocity[0];
	    //player.coords[1] += window.dt * TICK_RATE * player.velocity[1];
	    //player.coords[2] += window.dt * TICK_RATE * player.velocity[2];

	    printf("b4 player->velLeft: %f %f %f\n", player.velLeft[0], player.velLeft[1], player.velLeft[2]);
		//tmp remove l8r
		//glm_vec3_copy(player.coords, world.objList[9].coordinates);
		//player.camera.cameraPos[1] += 1.5f;
		bool isColliding = false;
		for (int i = 0; i < world.objCount; i++) {
			updateObj(&world.objList[i], window.dt, window.tick_float);
		}
		for (int i = 0; i < world.objCount; i++) {
			if (i != 0) {
				//world.objList[i].rotation = fmod(world.objList[i].rotation + window.dt * TICK_RATE, 360.0f);
				//world.objList[i].coordinates[0] = 15 * sin(glm_rad(window.tick_float));	//need a sudo +=
				//updateOBB(&world.objList[i].box, world.objList[i].coordinates, world.objList[i].scale_dim[0]/2.0f, world.objList[i].scale_dim[1]/2.0f, world.objList[i].scale_dim[2]/2.0f, world.objList[i].orientation_axis, world.objList[i].rotation);
			}
			if (ComputeResolveCollisions(&player, &world.objList[i], window.dt * TICK_RATE)) {
				isColliding = true;
			}
		}
		if (!isColliding) {
			player.in_air = true;
			printf("not colliding......................\n");
		}

		if (player.in_air) {
			printf("in air ___________________________\n");
			//this was why it worked...
			glm_vec3_copy(player.velFowardNormal, player.velFoward);
			glm_vec3_copy(player.velBackNormal, player.velBack);
			glm_vec3_copy(player.velLeftNormal, player.velLeft);
			glm_vec3_copy(player.velRightNormal, player.velRight);
			//glm_vec3_copy(player.velUpNormal, player.velUp);
			//player.velFowardNormal[3] = 0;
			//player.velBackNormal[3] = 0;
			//player.velUpNormal[3] = 0;
		} else {
			//glm_vec3_copy(player.velUpNormal, player.velUp);
			//player.velUpNormal[3] = 0;
		}

		//calc_orientation(&player.camera);
		//ComputePositionPlayer(&player, window.dt * TICK_RATE);
		calc_orientation(&player.camera);


		unsigned int viewLoc  = glGetUniformLocation(programID, "view");
		unsigned int projLoc  = glGetUniformLocation(programID, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)player.camera.lookAt_mat);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);

		for (unsigned int i = 0; i < world.objCount; i++) {
			//printf("%f, %f, %f\n", world.objList[i].coordinates[0], world.objList[i].coordinates[1], world.objList[i].coordinates[2]);
		    drawObject(world.objList[i], programID);
		}
		printf("coords: %f, %f, %f\n", player.coords[0], player.coords[1], player.coords[2]);
		//printf("theta: %f, phi %f\n", player.camera.theta, player.camera.phi);
		glfwPollEvents();
		action_callback();
		glfwSwapBuffers(window.handle);
	}

	//glfwDestroyWindow(window.handle);
	glfwTerminate();
}
