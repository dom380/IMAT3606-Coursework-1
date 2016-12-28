#include "Font.h"

Font::Font(FT_Library ftLib, char * fontPath)
{
	FT_New_Face(ft, fontPath, 0, &fontFace);
}
