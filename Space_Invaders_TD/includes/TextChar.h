#pragma once

#ifndef TEXT_CHAR_H
#define TEXT_CHAR_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

struct TextChar {
	GLuint textureID;
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // offset from baseline to left/top of glyph
	GLuint advance; // horizontal offset to advance to next glyph
};

typedef std::map<GLchar, TextChar> font;

#endif // TEXT_CHAR_H