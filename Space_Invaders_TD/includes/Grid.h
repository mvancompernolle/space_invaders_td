#pragma once

#ifndef GRID_STATE_H
#define GRID_STATE_H

#include "TowerTypes.h"
#include <vector>
#include <glm/glm.hpp>

struct GridState {
	bool taken; // taken by tower
	TOWER_TYPE towerType;
	int ent; // entity in the grid position
	glm::vec2 pos; // position in game world

	GridState() : taken( false ), towerType( TOWER_NONE ), ent( -1 ), pos( glm::vec2( 0.0f ) ) {}
};

typedef std::vector<std::vector<GridState>> Grid;
#endif //GRID_STATE_H