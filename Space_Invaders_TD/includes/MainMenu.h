#pragma once

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <GL/glew.h>
#include "Consts.h"
#include "Button.h"

class MainMenu {
public:
	MainMenu();
	~MainMenu();
	void init();
	STATE getMenuState();
	void render( GLfloat dt );
private:
	STATE state;
	GLuint width, height;
	Button bPlay, bExit, bRules;
};

#endif // MAIN_MENU_H