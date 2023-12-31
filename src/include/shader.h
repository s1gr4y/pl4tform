#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h" //<GLFW/glfw3.h>

// shader declarations
extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;

extern const char *vertexShaderSrcTXT;
extern const char *fragmentShaderSrcTXT;

extern const char *vertexLightShaderSrc;
extern const char *fragmentLightShaderSrc;

extern const char *vertexShaderSrc_Minimal;
extern const char *fragmentShaderSrc_Minimal;

extern const char *vertexShaderSrc_NoTexture;
extern const char *fragmentShaderSrc_NoTexture;

// vars to compile and link shaders
extern unsigned int vs1, fs1;				//vertex, fragment shader

// all shader ids
extern unsigned int programIDMain;			//compiled/linked programid
extern unsigned int programIDTxt;
extern unsigned int programIDLight;
extern unsigned int programIDSimple;
extern unsigned int programIDNoTexture;

//void loadShaders(const char* vsSource, const char* fsSource);
void loadShaders(const char* vsSource, const char* fsSource, unsigned int* programID);
void deleteUnbindShaders(unsigned int* vs, unsigned int* fs, unsigned int* programID); 

#endif	//end shader.h
