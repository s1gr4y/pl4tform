#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct Window {
    GLFWwindow *handle;
    int wid, high;
    char* title;
    unsigned long long int tick;
    double tick_float;

    double curr_time;
    double prev_time;
    double dt;									//delta time
    double time_passed;					//how long program has been running
};

//GLFWwindow* window;

int Window_init(int wid, int high, char* title);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void generateTexture(unsigned int* txtIndex, const char* file_name);
void window_loop();

// global window
extern struct Window window;
extern const int TICK_RATE;

#endif	//end WIN_ENGINE
