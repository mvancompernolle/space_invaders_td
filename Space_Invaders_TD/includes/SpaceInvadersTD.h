#pragma once

#ifndef SPACE_INVADERS_TD_H
#define SPACE_INVADERS_TD_H

#include "consts.h"
#include "systems/System.h"
#include "systems/CollisionSystem.h"
#include "systems/ShootSystem.h"
#include "systems/PathSystem.h"
#include "systems/DamageAuraSystem.h"
#include "Grid.h"
#include "World.h"
#include "Button.h"
#include "Textbox.h"
#include <vector>
#include <memory>

// maximum should be 18 by 48 = 864 tiles
const unsigned int NUM_GRID_ROWS = 12;
const unsigned int NUM_GRID_COLS = 32;
const glm::vec3 GRID_COLOR( 0.0f, 0.2f, 0.5f );
const glm::vec3 GRID_COLOR2( 0.0f );

enum TD_STATE {
	TD_PLAYING, TD_MENU, TD_LOSE, TD_WIN, TD_EXIT
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
	unsigned money, numEnemiesLeft, numLives, numRounds;
	int currRound;
	std::vector<unsigned> spawners, despawners;
	std::vector<System*> systems;
	World world;
	PathSystem* path;
	CollisionSystem collisionSystem;
	ShootSystem* shootSystem;
	DamageAuraSystem* dmgAuraSystem;

	// menu buttons
	std::vector<Button*> buttons;
	bool updateButtons;
	Button bStartRound, bMainMenu, bQuit, bPlaceWall;
	Button bUpgradeTrue, bUpgradeVoid, bUpgradePlasma, bUpgradeIce, bSellTower;
	Button bTrueVoid, bTruePlasma, bTrueIce;
	Button bVoidTrue, bVoidPlasma, bVoidIce;
	Button bPlasmaTrue, bPlasmaVoid, bPlasmaIce;
	Button bIceTrue, bIceVoid, bIcePlasma;
	Textbox* tbTowerInfo;
	std::unordered_map<TOWER_TYPE, std::vector<Button*>> towerButtons;

	void placeBaseTower( unsigned x, unsigned y );
	void handleCollisionEvents();
	void loadLevel( int level );
	void initMenuButtons();
	bool towerIsSelected() const;
	void showButtons( bool show );
	void updateSystem( System* system, int thread, int numThreads, float dt );
	void attemptToSlow( unsigned reciever, unsigned dealer, const SlowInfo& slowInfo );
	void startRound();
};

#endif SPACE_INVADERS_TD_H