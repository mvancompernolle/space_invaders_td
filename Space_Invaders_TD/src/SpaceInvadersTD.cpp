#include "SpaceInvadersTD.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "EntityFactory.h"
#include "MovementSystem.h"
#include "AISystem.h"
#include <ctime>
#include <cassert>

SpaceInvadersTD::SpaceInvadersTD() {
	// load assets
	ResourceManager::loadTexture( "enemy.png", GL_TRUE, "enemy" );
	ResourceManager::loadTexture( "tower_base.png", GL_TRUE, "tower_base" );
}


SpaceInvadersTD::~SpaceInvadersTD() {
	for ( int i = 0; i < systems.size(); ++i ) {
		delete systems[i];
	}
}

void SpaceInvadersTD::init() {
	// create grid
	float gridSize = (float)GAME_WIDTH / NUM_GRID_COLS;
	grid.resize( NUM_GRID_ROWS );
	for ( int i = 0; i < NUM_GRID_ROWS; ++i ) {
		grid[i].resize( NUM_GRID_COLS );
		for ( int j = 0; j < NUM_GRID_COLS; ++j ) {
			grid[i][j].taken = false;
			grid[i][j].pos = glm::vec2( gridSize * i, gridSize * j );
		}
	}

	// spawn an enemy
	EntityFactory::createEnemy();

	// spawn towers
	srand( time( NULL ) );
	glm::uvec2 dest = glm::uvec2( rand() % ( NUM_GRID_COLS / 4 ) + NUM_GRID_COLS / 4 * 3, rand() % ( NUM_GRID_ROWS / 3 ) + NUM_GRID_ROWS / 3 * 2 );
	for ( int i = 0; i < 400; ++i ) {
		glm::uvec2 pos = glm::uvec2( rand() % NUM_GRID_COLS, rand() % NUM_GRID_ROWS );
		if ( pos != dest )
			placeBaseTower( pos.x, pos.y );
	}

	// create systems
	systems.push_back( new MovementSystem);
	systems.push_back( new AISystem );
	AISystem* ai = (AISystem*) systems[1];
	ai->calcOptimalPath( glm::uvec2( rand() % ( NUM_GRID_COLS / 4 ), rand() % ( NUM_GRID_ROWS / 3 ) ), dest, grid );

	// set game values
	currGridPulseTime = 0.0f;

	// load background
	ResourceManager::loadTexture( "space.jpg", GL_TRUE, "game_background" );
}

STATE SpaceInvadersTD::update( const float dt ) {

	currGridPulseTime += dt * 3.14;

	for ( auto& system : systems ) {
		for ( Entity& ent : EntityFactory::getData() ) {
			if ( system->condition( ent.componentTypes ) ) {
				system->update( ent, dt );
			}
		}
	}

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

	// render entities
	unsigned renderFlags = WORLD | RENDER;
	for ( Entity& ent : EntityFactory::getData() ) {
		if ( !( ( renderFlags ^ ent.componentTypes ) & renderFlags ) ) {
			ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( ent.render->textureName ), ent.world->pos,
				ent.world->size, ent.world->rotation );
		}
	}

	// render health bars
	renderFlags = WORLD | HEALTH;
	for ( Entity& ent : EntityFactory::getData() ) {
		if ( !( ( renderFlags ^ ent.componentTypes ) & renderFlags ) ) {
			ServiceLocator::getGraphics().draw2DBox( ent.world->pos - glm::vec2( 0.0f, ent.world->size.y * 0.2f ),
				glm::vec2( ent.world->size.x , ent.world->size.y * 0.2f ), glm::vec3( 1.0f, 0.0f, 1.0f ) );
			float padding = 1.0f;
			ServiceLocator::getGraphics().draw2DBox( ent.world->pos - glm::vec2( -padding, ent.world->size.y * 0.2f - padding ),
				glm::vec2( (ent.world->size.x - (2.0f * padding)) * (ent.health->currHP / ent.health->maxHP), ent.world->size.y * 0.2f - (2.0f * padding) ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		}
	}
}

void SpaceInvadersTD::placeBaseTower( unsigned x, unsigned y ) {
	assert( x >= 0 && x < NUM_GRID_COLS && y >= 0 && y < NUM_GRID_ROWS );

	// create the tower
	Entity* ent = EntityFactory::createBaseTower();
	float gridSize = GAME_WIDTH / NUM_GRID_COLS;
	ent->world->size = glm::vec2( gridSize );
	ent->world->pos = glm::vec2( x * gridSize, y * gridSize );

	// mark that position as taken
	grid[y][x].taken = true;
}