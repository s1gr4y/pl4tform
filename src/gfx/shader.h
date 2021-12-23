#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

const char *vertexShaderSource =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec4 aCol;\n"
	"out vec4 vertexCol;\n"
    "void main()\n"
    "{\n"
	"	vertexCol = vec4(aCol.r, aCol.g, aCol.b, aCol.a);\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
    "out vec4 FragColor;\n"
	"in vec4 vertexCol;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vertexCol;\n"
    "}\n\0";

unsigned int vs, fs;		//vertex, fragment shader
unsigned int programID;		//compiled/linked programid

void loadShaders(const char* vsSource, const char* fsSource);

#endif	//end shader.h
