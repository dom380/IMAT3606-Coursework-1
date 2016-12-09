#include "..\..\include\GUI\Font.h"

Font::Font(Font * font)
{
	ft = font->ft;
	fontFace = font->fontFace;

}

Font::Font(FT_Library ftLib, char * fontPath)
{
	FT_New_Face(ft, fontPath, 0, &fontFace);
}

void Font::compile()
{
	if (compiled) return;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable OpenGLs byte alignment restriction as Freetype uses 1 byte colour (grey scale)
	Character character;
	for (GLubyte charByte = 0; charByte < 128; ++charByte) {
		FT_Load_Char(fontFace, charByte, FT_LOAD_RENDER);
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			fontFace->glyph->bitmap.width,
			fontFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			fontFace->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//store character in map for use
		character = Character(
			texture,
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			fontFace->glyph->advance.x
		);
		charMap.insert(std::pair<char,Character>(charByte, character));
	}
	//Free resources
	FT_Done_Face(fontFace);
	FT_Done_FreeType(ft);
	compiled = true;
}
