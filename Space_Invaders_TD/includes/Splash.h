#pragma once

#ifndef SPLASH_H
#define SPLASH_H

#include <GL/glew.h>
#include <string>
#include "Consts.h"

class Splash {
public:
					Splash();
					~Splash();
	STATE			update( GLfloat dt );
	void			render();
private:
	std::string		title, gameBy;
	GLfloat			currentTime, dt, charCooldown;
	GLuint			numTitleChars, numByChars;
	const GLfloat	animationTime = 0.075f;
	const GLfloat	splashTime = 5.0f;
	const GLuint	width, height;
};

#endif // SPLASH_H