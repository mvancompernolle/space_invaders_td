#pragma once

#ifndef SPACE_INVADERS_TD_H
#define SPACE_INVADERS_TD_H

#include "consts.h"
#include "System.h"
#include "Grid.h"
#include <vector>
#include <memory>

const unsigned int NUM_GRID_ROWS = 15;
const unsigned int NUM_GRID_COLS = 40;
const glm::vec3 GRID_COLOR( 0.0f, 0.2f, 0.5f );
const glm::vec3 GRID_COLOR2( 0.0f );

class SpaceInvadersTD {
public:
	SpaceInvadersTD();
	~SpaceInvadersTD();
	void init();
	STATE update( const float dt );
	void placeBaseTower( unsigned x, unsigned y );
	void render();
private:
	const float GRID_PULSE_TIME = 2.0f;
	const float GRID_PULSE_AMOUNT = 0.05f;
	float currGridPulseTime;
	Grid grid;
	std::vector<System*> systems;
	std::vector<Entity> entities;
};

#endif SPACE_INVADERS_TD_H