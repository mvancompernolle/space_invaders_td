#pragma once

#ifndef RULES_H
#define RULES_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Consts.h"
#include "Button.h"
#include "Textbox.h"

class Rules {
public:
					Rules();
					~Rules();
	void			init();
	STATE			update();
	void			render();
private:
	GLuint			width, height;
	Textbox			tbRules;
	Button			bBack;
	STATE			current;
};

#endif // RULES_H