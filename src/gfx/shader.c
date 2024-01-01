#include "shader.h"

unsigned int vs1, fs1;			//vertex, fragment shader
unsigned int programIDMain;			//compiled/linked programid

//unsigned int vs2d, fs2d;		//vs, fs for text.
unsigned int programIDTxt;		//compiled/linked programid for text

unsigned int programIDLight;	//compiled/linked programid for light

unsigned int programIDSimple;	//compiled/linked programid for basic shader

const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aNormal;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	//"out vec4 vertexCol;\n"
	"out vec3 FragPos;\n"
	"out vec2 TexCoord;\n"
	"out vec3 Normal;\n"
	//"uniform mat4 transform;\n"	//changed transform
	"uniform mat3 matrixNormal;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;"
	"void main()\n"
	"{\n"
	//"	vertexCol = vec4(aCol.r, aCol.g, aCol.b, aCol.a);\n"
    "	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" //vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"	TexCoord = aTexCoord;\n"				//mat3(transpose(inverse(model))) * aNormal;\n
	"	Normal = matrixNormal * aNormal;\n"	//normals rotate as the obj does, inefficient on gpu, prefer to do this in cpu then pass the result into shader
	"	FragPos = vec3(model * vec4(aPos, 1.0));\n"
	"}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
	"#define MAX_LIGHTS 64\n"
	"out vec4 FragColor;\n"
	
	"struct Material {\n"
	"    	sampler2D texture1;\n"
	"    	sampler2D texture2;\n"
	"    	float shininess;\n"
	"};\n"
	"struct DirLight {\n"	//the sun.
	"    	vec3 direction;\n"
	"    	vec3 ambient;\n"
	"    	vec3 diffuse;\n"
	"		vec3 specular;\n"
	"};\n"
	"struct PointLight {\n"
	"	vec3 position;\n"
	"	float constant;\n"
	"	float linear;\n"
	"	float quadratic;\n"
	"	vec3 ambient;\n"
	"	vec3 diffuse;\n"
	"	vec3 specular;\n"
	"};\n"

	"struct SpotLight {\n"
	"	vec3 position;\n"
	"	vec3 direction;\n"
	"	float cutOff;\n"
	"	float outerCutOff;\n"
	"	float constant;\n"
	"	float linear;\n"
	"	float quadratic;\n"
	
	"	vec3 ambient;\n"
	"	vec3 diffuse;\n"
	"	vec3 specular;\n"
	"};\n"
	
	"uniform int LIGHT_CAP;\n"
	"uniform int using_color;\n"

	"in vec3 FragPos;\n"
	"in vec3 Normal;\n"
	"in vec2 TexCoord;\n"

	"uniform vec3 viewPos;\n"
	"uniform DirLight dirLight;\n"
	"uniform PointLight pointLights[MAX_LIGHTS];\n"
	"uniform SpotLight spotLight;\n"
	"uniform Material material;\n"
	"uniform vec3 color"

	//"uniform sampler2D texture1;\n"
	//"uniform sampler2D texture2;\n"
	"uniform float mixer;\n"
	
	"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
	"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
	"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
	
	"void main()\n"
	"{\n"
	"vec3 norm = normalize(Normal);\n"
    "vec3 viewDir = normalize(viewPos - FragPos);\n"
	//"	FragColor = vec4(result, 1.0);\n"
	//" FragColor = vertexCol;\n"						//only color
	//"	FragColor = texture(ourTexture, TexCoord);\n"	//just texture
	//"	FragColor = texture(ourTexture, TexCoord) * vec4(vertexCol);\n"	//add rgb
	//"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixer); * result\n"
	"vec3 result = CalcDirLight(dirLight, norm, viewDir);\n"
	"for(int i = 0; i < LIGHT_CAP; i++) {\n"
	"	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"
	"}\n"
	"result += CalcSpotLight(spotLight, norm, FragPos, viewDir);\n"
	"FragColor = vec4(result, 1.0);\n"
	"}\n"
	
	//mix(texture(material.texture1, TexCoord), texture(material.texture2, TexCoord), mixer)
	//get text color based on universal dir light source.
	"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
	"{\n"
	"	vec3 lightDir = normalize(-light.direction);\n"
	"	// diffuse shading\n"
	"	float diff = max(dot(normal, lightDir), 0.0);\n"
	"	// specular shading\n"
	"	vec3 reflectDir = reflect(-lightDir, normal);\n"
	"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
	"	// combine results\n"
	"	vec3 ambient = light.ambient * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 specular = light.specular * spec * vec3(texture(material.texture1, TexCoord));\n"
	"	return (ambient + diffuse + specular);\n"
	"}\n"
	
	
	//calculates the color when using a point light.
	"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
	"{\n"
	"	vec3 lightDir = normalize(light.position - fragPos);\n"
	"	// diffuse shading\n"
	"	float diff = max(dot(normal, lightDir), 0.0);\n"
	"	// specular shading\n"
	"	vec3 reflectDir = reflect(-lightDir, normal);\n"
	"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
	"	// attenuation\n"
	"	float distance = length(light.position - fragPos);\n"
	"	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"
	"	// combine results\n"
	"	vec3 ambient = light.ambient * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 specular = light.specular * spec * vec3(texture(material.texture1, TexCoord));\n"
	"	ambient *= attenuation;\n"
	"	diffuse *= attenuation;\n"
	"	specular *= attenuation;\n"
	"	return (ambient + diffuse + specular);\n"
	"}\n"
	
	"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
	"{\n"
	"	vec3 lightDir = normalize(light.position - fragPos);\n"
	"	// diffuse shading\n"
	"	float diff = max(dot(normal, lightDir), 0.0);\n"
	"	// specular shading\n"
	"	vec3 reflectDir = reflect(-lightDir, normal);\n"
	"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
	"	// attenuation\n"
	"	float distance = length(light.position - fragPos);\n"
	"	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
	"	// spotlight intensity\n"
	"	float theta = dot(lightDir, normalize(-light.direction)); \n"
	"	float epsilon = light.cutOff - light.outerCutOff;\n"
	"	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n"
	"	// combine results\n"
	"	vec3 ambient = light.ambient * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture1, TexCoord));\n"
	"	vec3 specular = light.specular * spec * vec3(texture(material.texture1, TexCoord));\n"
	"	ambient *= attenuation * intensity;\n"
	"	diffuse *= attenuation * intensity;\n"
	"	specular *= attenuation * intensity;\n"
	"	return (ambient + diffuse + specular);\n"
	"}\n";
	
	
//light shaders
const char *vertexLightShaderSrc =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"}\n";
	
const char *fragmentLightShaderSrc =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0);\n"
	"}\n";

//text shaders
const char *vertexShaderSrcTXT =
	"#version 330 core\n"
	"layout (location = 0) in vec4 vertex;\n" // <vec2 pos, vec2 tex>
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;\n"
	"}\n";

const char *fragmentShaderSrcTXT =
	"#version 330 core\n"
	"in vec2 TexCoords;\n" // <vec2 pos, vec2 tex>
	"out vec4 color;\n"
	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"
	"void main()\n"
	"{\n"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	"	color = vec4(textColor, 1.0) * sampled;\n"
	"}\n";

//default shaders
const char *vertexShaderSrc_Minimal =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" //vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSrc_Minimal =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec3 color;\n"
    "void main()\n"
    "{\n"
	"	FragColor = vec4(color, 1.0);\n"
    "}\n\0";

//light but no texture
const char *vertexShaderSrc_NoTexture =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aNormal;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" //vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char *fragmentShaderSrc_NoTexture =
	"#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = vertexCol;\n"						//only color
	//"	FragColor = texture(ourTexture, TexCoord);\n"	//just texture
	//"	FragColor = texture(ourTexture, TexCoord) * vec4(vertexCol);\n"	//add rgb
	"	FragColor = vec4(0.4f, 0.5f, 0.7f, 1.0f);\n"
    "}\n\0";

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

