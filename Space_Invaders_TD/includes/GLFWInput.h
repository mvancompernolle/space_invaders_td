#pragma once

#ifndef GLFWINPUT_H
#define GLFWINPUT_H

#include "Input.h"
#include <GLFW/glfw3.h>

class GLFWInput : public Input{
public:
	GLFWInput();
	~GLFWInput();
};

#endif GLFWINPUT_H