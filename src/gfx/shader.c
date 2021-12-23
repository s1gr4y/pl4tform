#include "shader.h"



void loadShaders(const char* vsSource, const char* fsSource) {
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);

	programID = glCreateProgram();
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);

	glLinkProgram(programID);

	//delete everything
	glDetachShader(programID, vs);
	glDetachShader(programID, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

