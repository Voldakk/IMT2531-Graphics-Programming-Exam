#include "Text.hpp"

#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H


void Text::Init()
{
	std::cout << "Text::Init \n";
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "./assets/fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
}
