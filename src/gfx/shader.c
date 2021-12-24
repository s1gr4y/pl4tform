#include "shader.h"

const char *vertexShaderSource =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec4 aCol;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec4 vertexCol;\n"
	"out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
	"	vertexCol = vec4(aCol.r, aCol.g, aCol.b, aCol.a);\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"   TexCoord = aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
    "out vec4 FragColor;\n"
	"in vec4 vertexCol;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = vertexCol;\n"						//only color
	"	FragColor = texture(ourTexture, TexCoord);\n"	//just texture
	//"	FragColor = texture(ourTexture, TexCoord) * vec4(vertexCol);\n"	//add rgb
    "}\n\0";

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

