#include "window.h"

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
	window.inFocus = true;
	window.ESC_held = false;

	window.fps_tick_counter = 0;
	window.frame_counter = 0;

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

	initWorld();
	//loadShaders(vertexShaderSource, fragmentShaderSource);
	loadShaders(vertexShaderSource, fragmentShaderSource, &programIDMain);				//main shader + program
	loadShaders(vertexShaderSrcTXT, fragmentShaderSrcTXT, &programIDTxt);				//secondary shader + program
	loadShaders(vertexLightShaderSrc, fragmentLightShaderSrc, &programIDLight);			//light shader + program
	loadShaders(vertexShaderSrc_Minimal, fragmentShaderSrc_Minimal, &programIDSimple);	//simple shader + program
	loadShaders(vertexShaderSrc_Depth, fragmentShaderSrc_Depth, &programIDDepthShader);	//depth shader + program
	loadShaders(vertexShaderSrc_DepthQuad, fragmentShaderSrc_DepthQuad, &programIDDepthQuadShader);

	// set up shadows
	setupQuadVAO_VBO();
	generateDepthFBO();
	
	glUseProgram(programIDMain);
	//glUseProgram(programIDTxt);
	
	//for text to render
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadFont();
	generateVAO_VBO_text();
	
	
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
	float maxSpeed = CAM_SPEED * 2.5;
	// if key is pressed down do stuff.
	if (window.keyboard.keys[GLFW_KEY_ESCAPE]) {
		//glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
		if (window.ESC_held == false) {	// first time pressing ESC so we will wait a tick and if still pressing after 
			window.ESC_held = true;
			if (window.inFocus) {
				glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursorPosCallback(window.handle, NULL);
				window.inFocus = false;
			} else {
				glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPosCallback(window.handle, mouse_callback);
				window.inFocus = true;
			}
		}
	} else {
		window.ESC_held = false;
	}

	if (window.keyboard.keys[GLFW_KEY_W]) {
		vec3 temp;
		glm_vec3_zero(temp);
		//glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		temp[0] = cos(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		temp[2] = sin(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		//glm_vec3_add(player.velocity, temp, player.velocity);
		if (glm_vec3_norm(player.velMove) >= maxSpeed) {
			vec3 temp2;
			glm_vec3_zero(temp2);
			glm_vec3_copy(player.velMove, temp2);
			glm_vec3_normalize(temp2);
			float f = glm_vec3_norm(temp);
			glm_vec3_scale(temp2, f, temp2);
			glm_vec3_sub(player.velMove, temp2, player.velMove);
		}
		if (player.in_air == false) {
			glm_vec3_add(player.velMove, temp, player.velMove);
		} else {
			glm_vec3_add(player.velMove, temp, player.velMove);
			//glm_vec3_scale(temp, 0.2f, temp);
			//glm_vec3_add(player.velMoveNormal, temp, player.velMoveNormal);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_S]) {
		//glm_vec3_scale(camera.cameraFront, (window.dt * CAM_SPEED) * TICK_RATE, temp);
		vec3 temp;
		glm_vec3_zero(temp);
		temp[0] = cos(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		temp[2] = sin(glm_rad(player.camera.theta)) * (window.dt * CAM_SPEED) * TICK_RATE;
		//glm_vec3_sub(player.velocity, temp, player.velocity);
		if (glm_vec3_norm(player.velMove) >= maxSpeed) {
			vec3 temp2;
			glm_vec3_zero(temp2);
			glm_vec3_copy(player.velMove, temp2);
			glm_vec3_normalize(temp2);
			float f = glm_vec3_norm(temp);
			glm_vec3_scale(temp2, f, temp2);
			glm_vec3_sub(player.velMove, temp2, player.velMove);
		}
		if (player.in_air == false) {
			glm_vec3_sub(player.velMove, temp, player.velMove);
		} else {
			glm_vec3_sub(player.velMove, temp, player.velMove);
			//glm_vec3_scale(temp, 0.2f, temp);
			//glm_vec3_negate(temp);
			//glm_vec3_add(player.velMoveNormal, temp, player.velMoveNormal);
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
		if (glm_vec3_norm(player.velMove) >= maxSpeed) {
			vec3 temp2;
			glm_vec3_zero(temp2);
			glm_vec3_copy(player.velMove, temp2);
			glm_vec3_normalize(temp2);
			float f = glm_vec3_norm(temp);
			glm_vec3_scale(temp2, f, temp2);
			glm_vec3_sub(player.velMove, temp2, player.velMove);
		}
		if (player.in_air == false) {
			glm_vec3_sub(player.velMove, temp, player.velMove);
		} else {
			glm_vec3_sub(player.velMove, temp, player.velMove);
			//glm_vec3_scale(temp, 0.2f, temp);
			//glm_vec3_negate(temp);
			//glm_vec3_add(player.velMoveNormal, temp, player.velMoveNormal);
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
		if (glm_vec3_norm(player.velMove) >= maxSpeed) {
			vec3 temp2;
			glm_vec3_zero(temp2);
			glm_vec3_copy(player.velMove, temp2);
			glm_vec3_normalize(temp2);
			float f = glm_vec3_norm(temp);
			glm_vec3_scale(temp2, f, temp2);
			glm_vec3_sub(player.velMove, temp2, player.velMove);
		}
		if (player.in_air == false) {
			glm_vec3_add(player.velMove, temp, player.velMove);
		} else {
			glm_vec3_add(player.velMove, temp, player.velMove);
			//glm_vec3_scale(temp, 0.2f, temp);
			//glm_vec3_add(player.velMoveNormal, temp, player.velMoveNormal);
		}
	}
	if (window.keyboard.keys[GLFW_KEY_SPACE] && player.in_air == false && player.jumping == false) {	// && player.in_air == true?	//so we don't inf jump but lol y not.
		vec3 temp;
		glm_vec3_zero(temp);
		temp[1] = CAM_SPEED * 6;	//3.5
		//glm_vec3_add(player.velocity, temp, player.velocity);
		glm_vec3_copy(temp, player.velUp);
		//player.in_air = true;
		vec3 tmp2 = GLM_VEC3_ZERO_INIT;
		glm_vec3_copy(player.velMoveNormal, tmp2);
		tmp2[1] = 0.0f;
		glm_vec3_copy(tmp2, player.velMove);
		player.jumping = true;
		//printf("JUMPING: %f, %f, %f\n", player.velUp[0], player.velUp[1], player.velUp[2]);
	}
	if (player.in_air == false) {
		ApplyGroundResistance(player.velMove, window.dt * TICK_RATE);
	} else {
		ApplyAirResistance(player.velMove, window.dt * TICK_RATE);
	}
	///*
	/*if (glm_vec3_norm(player.velMove) >= maxSpeed) {
		glm_normalize(player.velMove);
		glm_vec3_scale(player.velMove, maxSpeed, player.velMove);
	}
	if (glm_vec3_norm(player.velMoveNormal) >= maxSpeed) {
		glm_normalize(player.velMoveNormal);
		glm_vec3_scale(player.velMoveNormal, maxSpeed, player.velMoveNormal);
	}
	*/

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

	//calc_orientation(&player.camera);
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
	window.tick_float = 0.0f;
	window.curr_time = glfwGetTime();
	window.prev_time = glfwGetTime();
	window.dt = 0.0;							//delta time
	window.time_passed = 0.0;					//how long program has been running
	
	
	while(!glfwWindowShouldClose(window.handle)) {
		glfwPollEvents();
		action_callback();

		//get delta time
		window.curr_time = glfwGetTime();
		//printf("current time: %f\n",window.curr_time);
		//printf("previous time: %f\n",window.prev_time);
		double prevTimeTmp = window.tick_float;
		window.dt = window.curr_time - window.prev_time;
		

		double prevTimePassed = window.time_passed;
		
		//prev time reset
		window.prev_time = window.curr_time;
		//printf("delta time: %f\n", window.dt);

		window.tick_float += window.dt * TICK_RATE;
		window.tick = (int)window.tick_float;
		window.time_passed += window.dt;

	    //printf("time: %f\n", window.time_passed);
		/*
	    if (window.tick >= 999999) { //3330
	    	glfwDestroyWindow(window.handle);
			window.handle = NULL;
	    	break;
	    }
		*/

	    //glClearColor(0.36f, 0.66f, 0.86f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw all after this
		
		glUseProgram(programIDMain);
		

	    //glUseProgram(programID);
	    //ComputePositionPlayer(&player, window.dt * TICK_RATE);

	    //player.coords[0] += window.dt * TICK_RATE * player.velocity[0];
	    //player.coords[1] += window.dt * TICK_RATE * player.velocity[1];
	    //player.coords[2] += window.dt * TICK_RATE * player.velocity[2];

		//tmp remove l8r
		//glm_vec3_copy(player.coords, world.objList[9].coordinates);
		//player.camera.cameraPos[1] += 1.5f;
		
		//Update obj pos
		//for (int i = 0; i < world.objCount; i++) {	//need to move obj later so vel doesn't missalign, but our model and obj aren't in sync
		//	updateObjPos(&world.objList[i], window.dt, window.tick_float);
		//}
		
		UpdatePlayerPos(&player, window.dt * TICK_RATE);	//this only preapplies gravity
		
		bool isColliding = false;
		for (int i = 0; i < world.objCount; i++) {
			if (i != 0) {
				//world.objList[i].rotation = fmod(world.objList[i].rotation + window.dt * TICK_RATE, 360.0f);
				//world.objList[i].coordinates[0] = 15 * sin(glm_rad(window.tick_float));	//need a sudo +=
				//updateOBB(&world.objList[i].box, world.objList[i].coordinates, world.objList[i].scale_dim[0]/2.0f, world.objList[i].scale_dim[1]/2.0f, world.objList[i].scale_dim[2]/2.0f, world.objList[i].orientation_axis, world.objList[i].rotation);
			}
			if (world.objList[i].hasCollision == true && ComputeResolveCollisions(&player, &world.objList[i], window.dt * TICK_RATE)) {
				isColliding = true;
			}
		}
		if (!isColliding) {
			player.in_air = true;
			player.is_grounded = false;
			//printf("not colliding......................\n");
		} else {
		
			//Update obj vel
			glm_vec3_zero(player.velAdded);
			for (int i = 0; i < world.objCount; i++) {	//need to move obj later so vel doesn't missalign, but our model and obj aren't in sync
				updateObjVel(&world.objList[i], window.dt, window.tick_float);
				//change player's velAdded
				if (player.objCollisionList[i] == true) {
					// should also check if adding would slow the player down, if it does add anyway
					if ((glm_vec3_norm(world.objList[i].velocity) + (CAM_SPEED * 2.5)) >= glm_vec3_norm(player.velocity)) {// prevents flying but hard to stick on top of objs
						glm_vec3_add(player.velAdded, world.objList[i].velocity, player.velAdded);
					}
				}
				player.objCollisionList[i] = false;
			}
		
		}
		for (int i = 0; i < world.objCount; i++) {
			player.objCollisionList[i] = false;
		}

		//calc_orientation(&player.camera);
		ComputePositionPlayer(&player, window.dt * TICK_RATE);
		calc_orientation(&player.camera);
		
		//Update obj vel
		for (int i = 0; i < world.objCount; i++) {	//need to move obj later so vel doesn't missalign, but our model and obj aren't in sync
			static float updDeg = 0.0f;
			static vec3 tmp = (vec3) {3.0f, 0.0f, 0.0f};
			static bool first = false;
			updateObjVel(&world.objList[i], window.dt, window.tick_float);
			updateObjPos(&world.objList[i], window.dt, window.tick_float);
		}
	
		//to change texture
		//glUseProgram(programIDMain);
		//glUniform1i(glGetUniformLocation(programIDMain, "shadowMap"), 3);
		drawAllObjects(true);

		drawAllObjects(false);

		//DrawQuad(programIDDepthQuadShader);	// for testing

		//drawFPS(prevTimePassed);
		
		//*/
		
		//printf("coords: %f, %f, %f\n", player.coords[0], player.coords[1], player.coords[2]);
		//printf("theta: %f, phi %f\n", player.camera.theta, player.camera.phi);
		glfwSwapBuffers(window.handle);
	}

	//glfwDestroyWindow(window.handle);
	glfwTerminate();
}


void drawAllObjects(bool shadowMode) {	// we can worry about breaking this up later
	//glEnable(GL_DEPTH_TEST);
	//unsigned int viewLoc  = glGetUniformLocation(programIDMain, "view");
	//unsigned int projLoc  = glGetUniformLocation(programIDMain, "projection");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)player.camera.lookAt_mat);
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);
	//currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	mat4 projection;
	glm_mat4_identity(projection);
	glm_perspective(glm_rad(90.0f), (float)window.wid / (float)window.high, 0.1f, 100.0f, projection);

	if (shadowMode) {
		// we should switch to a seprate list of light objects and another for normal objects but fine O(n^2) for now
		glUseProgram(programIDDepthShader);
		for (unsigned int i = 0; i < world.objCount; i++) {
			if (world.objList[i].lightSrc == true) {
				renderDepthMap(world.objList[i], programIDDepthShader);
				for (unsigned int i = 0; i < world.objCount; i++) {
					if (world.objList[i].lightSrc == false) {
						drawObjectSimple(world.objList[i], world.meshList, programIDDepthShader);
						//Camera tmp;
						//glm_vec3_copy(world.objList[i].coordinates, tmp.cameraPos);
						//glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, tmp->cameraFront);
						//drawObject(world.objList[i], world.meshList, programIDSimple, NULL, 0, player.camera);
					}
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				// reset viewport
        		glViewport(0, 0, window.wid, window.high);
        		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}
		return;	// kill early since this is first pass
	}

	for (unsigned int i = 0; i < world.objCount; i++) {
		//printf("%f, %f, %f\n", world.objList[i].coordinates[0], world.objList[i].coordinates[1], world.objList[i].coordinates[2]);
		if (world.objList[i].lightSrc == false) {
			if (world.objList[i].type > 6) {	// joke is no type is >6. Temp disabling this
				glUseProgram(programIDSimple);
				glEnable(GL_DEPTH_TEST);
				unsigned int TviewLoc  = glGetUniformLocation(programIDSimple, "view");
				unsigned int TprojLoc  = glGetUniformLocation(programIDSimple, "projection");
				glUniformMatrix4fv(TviewLoc, 1, GL_FALSE, (float*)player.camera.lookAt_mat);
				glUniformMatrix4fv(TprojLoc, 1, GL_FALSE, (float*)projection);
				//ALTdrawObject(world.objList[i], programIDSimple);
				drawObjectSimple(world.objList[i], world.meshList, programIDSimple);
			} else {
				int count = 0;
				Object *closest_src_list = findClosestLightSrc(world.objList[i], &count);	//self doesn't matter since we know not light source
				glUseProgram(programIDMain);
				glEnable(GL_DEPTH_TEST);
				unsigned int viewLoc  = glGetUniformLocation(programIDMain, "view");
				unsigned int projLoc  = glGetUniformLocation(programIDMain, "projection");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)player.camera.lookAt_mat);
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);
				drawObject(world.objList[i], world.meshList, programIDMain, closest_src_list, count, player.camera);
				//printf("hy\n");
				free(closest_src_list);
			}
		} else {
			glUseProgram(programIDLight);
			glEnable(GL_DEPTH_TEST);
			unsigned int TviewLoc  = glGetUniformLocation(programIDLight, "view");
			unsigned int TprojLoc  = glGetUniformLocation(programIDLight, "projection");
			glUniformMatrix4fv(TviewLoc, 1, GL_FALSE, (float*)player.camera.lookAt_mat);
			glUniformMatrix4fv(TprojLoc, 1, GL_FALSE, (float*)projection);
			drawObjectLight(world.objList[i], world.meshList, programIDLight);
		}
	}
}

void drawFPS(double prevTimePassed) {
	window.fps = (1.0f/window.dt);
		//printf("fps num is %d\n", window.fps);
		const char tmpStr[] = "FPS: ";
		///*
		//printf("win: %f || prev: %f\n", window.time_passed, prevTimePassed);
		if ((int)window.time_passed != (int)prevTimePassed)	{	//if (window.tick != (int) prevTimeTmp) {	//inaccurate, should count all frames every 11 ticks
			//window.dtList[fps_tick_counter] = window.fps;
			window.fps_tick_counter++;
		}
		if (window.fps_tick_counter >= 1) {	//1/6 second has passed
			window.current_display_fps = window.frame_counter; //sum/10; //window.fps;
			window.fps_tick_counter = 0;
			window.frame_counter = 0;
		}
		
		//int fps = (1.0f/window.dt);
		//printf("current fps: %d\n", window.current_display_fps);
		int rTmp = window.current_display_fps;
		int tmpI = 0;
		while (rTmp > 0) {
			rTmp = rTmp/10;
			tmpI++;
		}
		rTmp = window.current_display_fps;
		int length = strlen(tmpStr) + tmpI;
		char* FPS_counter = (char*) malloc(sizeof(char) * (length + 1));
		strcpy(FPS_counter, tmpStr);
		for (int i = length - 1; i >= strlen(tmpStr); i--) {
			//printf("cond num is %ld\n", strlen(tmpStr) + tmpI);
			FPS_counter[i] = '0' + rTmp % 10;	//ascii 0 offset added.
			//printf("at i of %d, it is %c\n", i, FPS_counter[i]);
			rTmp /= 10;
		}
		FPS_counter[length] = 0;	//null terminator
		//printf("%s\n", FPS_counter);
		//printf("length of it is %ld\n", strlen(FPS_counter));
		RenderText(programIDTxt, FPS_counter, strlen(tmpStr) + tmpI, window.wid - 315.0f, window.high - 80.0f, 1.0f, (vec3){1.0f, 1.0f, 1.0f}, window.wid, window.high);
		
		free(FPS_counter);		// could consider not doing this every frame but will worry about later
		window.frame_counter++;
}
