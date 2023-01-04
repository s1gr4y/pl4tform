#ifndef FONT_H
#define FONT_H

#include "../../Dependencies/ft2build.h"
#include FT_FREETYPE_H

#include "../../Dependencies/glad/include/glad/glad.h" //<glad/glad.h> //"../../Dependencies/glad/include/glad/glad.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/cglm/cglm.h"
#include "../include/shader.h"

typedef struct Character {
    unsigned int textureID;  	//ID for texture
    vec2 size;       			//width, height
    vec2 bearing;    			//offset from baseline to left/top
    unsigned int advance; 		//offset to advance to next char
} Character;

unsigned VAOtxt, VBOtxt;

void generateVAO_VBO_text();
int loadFont();
void RenderText(unsigned int prgID, char* text, unsigned int textLength, float x, float y, float scale, vec3 color, float WinWidth, float Winheight);


extern Character charList[128];	//all ascii characters

#endif