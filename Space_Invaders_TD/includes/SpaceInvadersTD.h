#pragma once

#ifndef SPACE_INVADERS_TD_H
#define SPACE_INVADERS_TD_H

#include "consts.h"
#include "System.h"
#include <vector>

const unsigned int NUM_GRID_ROWS = 9;
const unsigned int NUM_GRID_COLS = 24;
const glm::vec3 GRID_COLOR( 0.0f, 0.2f, 0.5f );
const glm::vec3 GRID_COLOR2( 0.0f );

struct gridState {
	bool taken;
	unsigned int heuristic;
};

class SpaceInvadersTD {
public:
	SpaceInvadersTD();
	~SpaceInvadersTD();
	void init();
	STATE update( const float dt );
	void render();
private:
	const float GRID_PULSE_TIME = 2.0f;
	const float GRID_PULSE_AMOUNT = 0.05f;
	float currGridPulseTime;
	gridState grid[NUM_GRID_ROWS][NUM_GRID_COLS];
	std::vector<System> systems;
};

#endif SPACE_INVADERS_TD_H