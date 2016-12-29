#pragma once
#ifndef FONT_H
#define FONT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <map>
using std::map;
#include <gl\glm\glm\glm.hpp>
#include <Graphics.h>
class Graphics;
class Font {
public:
	Font() {};
	Font(Font* font);
	Font(FT_Library ftLib, char* fontPath, shared_ptr<Graphics>& graphics);
	void compile();
	class Character {
	public:
		Character() {};
		Character(unsigned int textureId, glm::ivec2 sizeVec, glm::ivec2 bearingVec, unsigned int charOffset) {
			texture = textureId;
			size = sizeVec;
			bearing = bearingVec;
			offset = charOffset;
		};
		unsigned int texture;    // ID handle of the glyph texture
		glm::ivec2 size;       // Size of glyph
		glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
		unsigned int offset;     // Offset to advance to next glyph
	};
	Character getChar(char c);
private:
	FT_Library ft;
	FT_Face fontFace;
	map<char, Character> charMap;
	bool compiled = false;
	shared_ptr<Graphics> graphics;
};

#endif // !FONT_H

