#pragma once

#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>
#include <glm/glm.hpp>

struct GridState {
	bool taken;
	int ent;
	glm::vec2 pos;

	GridState() : taken( false ), ent( -1 ), pos( glm::vec2( 0.0f ) ) {}
};

typedef std::vector<std::vector<GridState>> Grid;

#endif //GRID_STATE_H