#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include "Texture.h"
#include "string.h"
#include "TextChar.h"
#include "TextAlignment.h"

class Graphics {
public:
						Graphics();
	virtual				~Graphics();
	virtual void		setBackgroundColor( glm::vec3 color ) = 0;
	virtual void		renderText( const font& characters, std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color = glm::vec3( 1.0f ),
							HOR_ALIGNMENT horAlign = LEFT_ALIGNED, VERT_ALIGNMENT vertAlign = BELOW ) = 0;
	virtual void		draw2DTexture( const Texture& texture, glm::vec2 pos, glm::vec2 size, GLfloat rot, glm::vec4 color = glm::vec4( 1.0f ), GLboolean flipH = GL_FALSE ) = 0;
	virtual void		draw2DBox( glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLfloat rot = 0.0f ) = 0;
	virtual GLboolean	windowIsValid() const = 0;
	virtual void		swapBuffers() = 0;
	glm::vec2			getDimensions() const;
protected:
	GLuint				windowWidth, windowHeight;
};

#endif GRAPHICS_H