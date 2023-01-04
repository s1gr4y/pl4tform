#include "shader.h"

unsigned int vs1, fs1;			//vertex, fragment shader
unsigned int programIDMain;			//compiled/linked programid

//unsigned int vs2d, fs2d;		//vs, fs for text.
unsigned int programIDTxt;		//compiled/linked programid for text

const char *vertexShaderSource =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec4 aCol;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec4 vertexCol;\n"
	"out vec2 TexCoord;\n"
	//"uniform mat4 transform;\n"	//changed transform
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;"
    "void main()\n"
    "{\n"
	"	vertexCol = vec4(aCol.r, aCol.g, aCol.b, aCol.a);\n"
    "   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" //vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"   TexCoord = aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
    "out vec4 FragColor;\n"
	"in vec4 vertexCol;\n"
	"in vec2 TexCoord;\n"
	//"uniform sampler2D ourTexture;\n"
	"uniform sampler2D texture1;\n"
	"uniform sampler2D texture2;\n"
	"uniform float mixer;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = vertexCol;\n"						//only color
	//"	FragColor = texture(ourTexture, TexCoord);\n"	//just texture
	//"	FragColor = texture(ourTexture, TexCoord) * vec4(vertexCol);\n"	//add rgb
	"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixer);\n"
    "}\n\0";

//text shaders
const char *vertexShaderSrcTXT =
	"#version 330 core\n"
	"layout (location = 0) in vec4 vertex;\n" // <vec2 pos, vec2 tex>
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
		"gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0);\n"
		"TexCoords = vertex.zw;\n"
	"}\n";

const char *fragmentShaderSrcTXT =
	"#version 330 core\n"
	"in vec2 TexCoords;\n" // <vec2 pos, vec2 tex>
	"out vec4 color;\n"
	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"
	"void main()\n"
	"{\n"
		"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"color = vec4(textColor, 1.0) * sampled;\n"
	"}\n";

void loadShaders(const char* vsSource, const char* fsSource, unsigned int* programID) {
	unsigned int vs, fs;
	int success;
	char infoLog[512];
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);

	//check for shader compile errors
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("ERROR: Vertex shader compilation failed\n");
        printf("%s\n", infoLog);
    }

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);

    //check for shader compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("ERROR: Fragment shader compilation failed\n");
		printf("%s\n",infoLog);
    }

	*programID = glCreateProgram();
	glAttachShader(*programID, vs);
	glAttachShader(*programID, fs);

	glLinkProgram(*programID);

	//check linking errors
    glGetProgramiv(*programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*programID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf("%s\n", infoLog);
    }
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	//delete everything
	/*
	glDetachShader(programID, vs);
	glDetachShader(programID, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	*/
	//saving it since it is our default shader
}

//dead code
void deleteUnbindShaders(unsigned int* vs, unsigned int* fs, unsigned int* programID) {
	glDetachShader(*programID, *vs);
	glDetachShader(*programID, *fs);
	glDeleteShader(*vs);
	glDeleteShader(*fs);
}

