#pragma once

#ifndef GLFWGRAPHICS_H
#define GLFWGRAPHICS_H

#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "BoxRenderer.h"

class GLFWGraphics : public Graphics {
public:
						GLFWGraphics(std::string windowName ); // fullscreen constructor
						GLFWGraphics( std::string windowName, GLuint width, GLuint height ); // set dimensions constructor
						~GLFWGraphics();
	virtual void		setBackgroundColor( glm::vec3 color );
	virtual void		renderText( const font& characters, std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color = glm::vec3( 1.0f ),
								HOR_ALIGNMENT horAlign = LEFT_ALIGNED, VERT_ALIGNMENT vertAlign = BELOW );
	virtual void		draw2DTexture( const Texture& texture, glm::vec2 pos, glm::vec2 size, GLfloat rot, glm::vec4 color = glm::vec4( 1.0f ), GLboolean flipH = GL_FALSE );
	virtual void		draw2DBox( glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLfloat rot = 0.0f );
	virtual GLboolean	windowIsValid() const;
	virtual void		swapBuffers();
	GLFWwindow*			getWindow() const;
private:
	GLFWwindow			*window;
	SpriteRenderer		*spriteRenderer;
	TextRenderer		*textRenderer;
	BoxRenderer			*boxRenderer;

	void				init();
};

#endif //GLFWGRAPHICS_H