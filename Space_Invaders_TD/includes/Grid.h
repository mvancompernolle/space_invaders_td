#pragma once

#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>
#include <glm/glm.hpp>

enum TOWER_TYPE {
	TOWER_NONE, TOWER_WALL, 
	TOWER_TRUE, TOWER_VOID, TOWER_PLASMA, TOWER_ICE,
	TOWER_TRUE_VOID, TOWER_TRUE_PLASMA, TOWER_TRUE_ICE,
	TOWER_VOID_TRUE, TOWER_VOID_PLASMA, TOWER_VOID_ICE,
	TOWER_PLASMA_TRUE, TOWER_PLASMA_VOID, TOWER_PLASMA_ICE,
	TOWER_ICE_TRUE, TOWER_ICE_VOID, TOWER_ICE_PLASMA
};

struct GridState {
	bool taken; // taken by tower
	TOWER_TYPE towerType;
	int ent; // entity in the grid position
	glm::vec2 pos; // position in game world

	GridState() : taken( false ), towerType( TOWER_NONE ), ent( -1 ), pos( glm::vec2( 0.0f ) ) {}
};

typedef std::vector<std::vector<GridState>> Grid;
#endif //GRID_STATE_H