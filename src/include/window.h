#ifndef WINDOW_H
#define WINDOW_H

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h> //"../../Dependencies/glad/include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h" //<GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/stb_image.h"
#include "../../Dependencies/cglm/cglm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "../include/mesh.h"
#include "../include/game.h"
#include "../include/camera.h"
#include "../include/font.h"

struct Button {
	int pressed;
};

struct Keyboard {
	//arr (map mimic) of all possible key presses since GLFW_KEY_LAST is largest num
	int keys[GLFW_KEY_LAST];
};

struct Window {
    GLFWwindow *handle;
    int wid, high;
    char* title;
    unsigned long long int tick;
    double tick_float;

    //last x and y coords of mouse
    float lastX;
    float lastY;

	int current_display_fps;
	int fps;
    bool inFocus;
    bool ESC_held;
	
    double curr_time;
    double prev_time;
    double dt;							    //delta time
    double time_passed;					    //how long program has been running

    struct Keyboard keyboard;
};

//GLFWwindow* window;

int Window_init(int wid, int high, char* title);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void get_resolution(int *width, int *high);
void action_callback();
void mouse_callback(GLFWwindow* handle, double xpos, double ypos);
void window_loop();

// global window
extern struct Window window;
extern int firstMouse;
//extern struct Keyboard keyboard;

#endif	//end WIN_ENGINE
