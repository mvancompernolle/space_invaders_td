#pragma once

#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>
#include <glm/glm.hpp>

struct GridState {
	bool taken;
	glm::vec2 pos;
};

typedef std::vector<std::vector<GridState>> Grid;

#endif //GRID_STATE_H