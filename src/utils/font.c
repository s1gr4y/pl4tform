#include "font.h"

unsigned int VAOtxt, VBOtxt;
Character charList[128];
///*
void generateVAO_VBO_text() {
	//setup VAO/VBO of text images
	glGenVertexArrays(1, &VAOtxt);
	glGenBuffers(1, &VBOtxt);
	glBindVertexArray(VAOtxt);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtxt);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int loadFont() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		printf("Could not init freetype library\n");
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Resources/fonts/Minecraftia-Regular.ttf", 0, &face)) {
		printf("Failed to load font\n");
		return -1;
	}
	
	FT_Set_Pixel_Sizes(face, 0, 48);
	
	//load glyph to check if loads
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		printf("Failed to load glyph\n");
		return -1;
	}
	
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte-alignment restriction, prevent segfaults
  
	for (unsigned char c = 0; c < 128; c++) {
		//load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			printf("Failed to load glyph\n");
			continue;
		}
		//generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Character character;
		character.textureID = texture;
		glm_vec2_copy((vec2){face->glyph->bitmap.width, face->glyph->bitmap.rows}, character.size);
		glm_vec2_copy((vec2){face->glyph->bitmap_left, face->glyph->bitmap_top}, character.bearing);
		character.advance = face->glyph->advance.x;

		charList[c] = character;
	}
	//free it
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}


void RenderText(unsigned int prgID, char* text, unsigned int textLength, float x, float y, float scale, vec3 color, float WinWidth, float Winheight) {
	glUseProgram(prgID);
	glDisable(GL_DEPTH_TEST);	// make sure you reenable after
	mat4 projectionFLAT;
	glm_mat4_identity(projectionFLAT);
	glm_ortho(0.0f, WinWidth, 0.0f, Winheight, 0.0f, 100.0f, projectionFLAT);
	unsigned int projLoc2  = glGetUniformLocation(programIDTxt, "projection");
	glUniformMatrix4fv(projLoc2, 1, GL_FALSE, (float*)projectionFLAT);

	//enable
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3f(glGetUniformLocation(prgID, "textColor"), color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAOtxt);

    // iterate through all characters
    for (int i = 0; i < textLength; i++) {
		unsigned char c = text[i];
		//printf(" %d ", c);
        Character ch = charList[c];

        float xpos = x + ch.bearing[0] * scale;
        float ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

        float w = ch.size[0] * scale;
        float h = ch.size[1] * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
		//glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glUniform1i(glGetUniformLocation(prgID, "text"), 0);
		//change content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOtxt);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//advance cursor for next char (adv is in terms of 1/64 pixels), so bitshift by 6 to get value in pixels (2^6 = 64)
		x += (ch.advance >> 6) * scale;
    }
	//printf("\n");
    //glBindVertexArray(0);
    //glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
//*/