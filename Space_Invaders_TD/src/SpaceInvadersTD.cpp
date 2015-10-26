#include "SpaceInvadersTD.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include <ctime>

SpaceInvadersTD::SpaceInvadersTD() {
}


SpaceInvadersTD::~SpaceInvadersTD() {
}

void SpaceInvadersTD::init() {

	// set game values
	currGridPulseTime = 0.0f;

	// load background
	ResourceManager::loadTexture( "space.jpg", GL_TRUE, "game_background" );

	// reset grid
	for ( int i = 0; i < NUM_GRID_ROWS; ++i ) {
		for ( int j = 0; j < NUM_GRID_COLS; ++j ) {
			grid[i][j].taken = false;
			grid[i][j].heuristic = 0;
		}
	}
}

STATE SpaceInvadersTD::update( const float dt ) {

	currGridPulseTime += dt * 3.14;

	return GAME;
}

void SpaceInvadersTD::render() {
	// render background
	ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "game_background" ), glm::vec2( 0.0f ), glm::vec2( GAME_WIDTH, GAME_HEIGHT ), 0.0f );

	// render game grid lines
	float gridSize = (float) GAME_WIDTH / NUM_GRID_COLS;

	// render horizontal lines
	for ( int i = 1; i <= NUM_GRID_ROWS; ++i ) {
		ServiceLocator::getGraphics().draw2DBox( glm::vec2( 0.0f, gridSize * i - 1 ), glm::vec2( gridSize * NUM_GRID_COLS, 2.0f ), GRID_COLOR + std::sin( currGridPulseTime / GRID_PULSE_TIME ) * GRID_PULSE_AMOUNT );
	}
	// render vertical lines
	for ( int i = 1; i < NUM_GRID_COLS; ++i ) {
		ServiceLocator::getGraphics().draw2DBox( glm::vec2( gridSize * i - 1, 0.0f ), glm::vec2( 2.0f, gridSize * NUM_GRID_ROWS ), GRID_COLOR + std::sin( currGridPulseTime / GRID_PULSE_TIME ) * GRID_PULSE_AMOUNT );
	}
}
