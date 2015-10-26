#pragma once

#ifndef BOX_RENDERER_H
#define BOX_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class BoxRenderer {
public:
			BoxRenderer();
			~BoxRenderer();
	void	renderBox( glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLfloat rot = 0.0f );
private:
	Shader	shader;
	GLuint	VAO;
	void	initRenderData();
};

#endif //BOX_RENDERER_H