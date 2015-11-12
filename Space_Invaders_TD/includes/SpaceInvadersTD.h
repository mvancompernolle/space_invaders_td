#pragma once

#ifndef SPACE_INVADERS_TD_H
#define SPACE_INVADERS_TD_H

#include "consts.h"
#include "System.h"
#include "CollisionSystem.h"
#include "ShootSystem.h"
#include "PathSystem.h"
#include "Grid.h"
#include "World.h"
#include "Button.h"
#include <vector>
#include <memory>

// maximum should be 18 by 48 = 864 tiles
const unsigned int NUM_GRID_ROWS = 12;
const unsigned int NUM_GRID_COLS = 32;
const glm::vec3 GRID_COLOR( 0.0f, 0.2f, 0.5f );
const glm::vec3 GRID_COLOR2( 0.0f );

enum TD_STATE {
	TD_PLAYING, TD_MENU
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
	float gridSize;
	float currGridPulseTime;
	Grid grid;
	TD_STATE tdState;
	bool placeTowerMode;
	glm::ivec2 selectedGridPos;
	unsigned money, numEnemiesLeft, numLives;
	std::vector<unsigned> spawners, despawners;
	std::vector<System*> systems;
	World world;
	PathSystem* path;
	CollisionSystem collisionSystem;
	ShootSystem* shootSystem;

	// menu buttons
	std::vector<Button*> buttons;
	Button bStartRound, bPlaceWall;
	Button bUpgradeTrue, bUpgradeVoid, bUpgradePlasma, bUpgradeIce, bSellTower;

	void placeBaseTower( unsigned x, unsigned y );
	void handleCollisionEvents();
	void loadLevel( int level );
	void initMenuButtons();
	bool towerIsSelected() const;
	void showButtons( bool show );
};

#endif SPACE_INVADERS_TD_H