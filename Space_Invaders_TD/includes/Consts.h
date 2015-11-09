#pragma once

#ifndef CONSTS_H
#define CONSTS_H

#include <glm/glm.hpp>

enum STATE { SPLASH, MAIN_INIT, RULES_INIT, RULES, MAIN, PLAY, GAME, EXIT };

#define COLOR_INFO		glm::vec3(0.0f, 0.0f, 1.0f)
#define COLOR_ACTION	glm::vec3(0.0f)
#define	COLOR_NEGATIVE	glm::vec3(0.5f, 0.0f, 0.0f)
#define	COLOR_POSITIVE	glm::vec3(0.0f, 0.5f, 0.0f)
#define GAME_WIDTH		1920
#define GAME_HEIGHT		1080
#define PI				3.14159265359	

#endif // CONSTS_H