#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h" //<GLFW/glfw3.h>

extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;

extern const char *vertexShaderSrcTXT;
extern const char *fragmentShaderSrcTXT;

extern const char *vertexLightShaderSrc;
	
extern const char *fragmentLightShaderSrc;

extern unsigned int vs1, fs1;				//vertex, fragment shader
extern unsigned int programIDMain;			//compiled/linked programid

extern unsigned int vs2d, fs2d;				//unneeded, only both programIDs
extern unsigned int programIDTxt;

extern unsigned int programIDLight;

//void loadShaders(const char* vsSource, const char* fsSource);
void loadShaders(const char* vsSource, const char* fsSource, unsigned int* programID);
void deleteUnbindShaders(unsigned int* vs, unsigned int* fs, unsigned int* programID); 

#endif	//end shader.h
