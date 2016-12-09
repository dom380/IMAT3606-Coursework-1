#pragma once
#ifndef FONT_H
#define FONT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
using std::map;
#include <gl\glew.h>
#include <gl\glm\glm\glm.hpp>

class Font {
private:
	FT_Library ft;
	FT_Face fontFace;
	class Character {
	public:
		Character() {};
		Character(GLuint textureId, glm::ivec2 sizeVec, glm::ivec2 bearingVec, GLuint charOffset) {
			texture = textureId;
			size = sizeVec;
			bearing = bearingVec;
			offset = charOffset;
		}
		GLuint     texture;    // ID handle of the glyph texture
		glm::ivec2 size;       // Size of glyph
		glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
		GLuint     offset;     // Offset to advance to next glyph
	};
	map<char, Character> charMap;
	bool compiled = false;
	
public:
	Font() {};
	Font(Font* font);
	Font(FT_Library ftLib, char* fontPath);
	void compile();
};

#endif // !FONT_H

