#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;

unsigned int vs, fs;		//vertex, fragment shader
unsigned int programID;		//compiled/linked programid

void loadShaders(const char* vsSource, const char* fsSource);

#endif	//end shader.h
