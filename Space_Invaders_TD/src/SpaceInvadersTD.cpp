#include "SpaceInvadersTD.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "EntityFactory.h"
#include "systems/MovementSystem.h"
#include "systems/SpawnSystem.h"
#include "systems/PlayerInputSystem.h"
#include "systems/HealthSystem.h"
#include "systems/FollowSystem.h"
#include "systems/SlowedSystem.h"
#include "systems/RotationSystem.h"
#include <cstring>
#include <ctime>
#include <cassert>
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>

#define MULTITHREADED false

SpaceInvadersTD::SpaceInvadersTD() {
	// load assets
	ResourceManager::loadTexture( "hud.png", GL_TRUE, "hud" );
	ResourceManager::loadTexture( "player.png", GL_TRUE, "player" );
	ResourceManager::loadTexture( "enemy.png", GL_TRUE, "enemy" );
	ResourceManager::loadTexture( "tower_base.png", GL_TRUE, "tower_base" );
	ResourceManager::loadTexture( "place_tower_indicator.png", GL_TRUE, "place_tower_indicator" );
	ResourceManager::loadTexture( "grid_hover.png", GL_TRUE, "grid_hover" );
	ResourceManager::loadTexture( "grid_selected.png", GL_TRUE, "grid_selected" );
	ResourceManager::loadTexture( "spawn_portal.png", GL_TRUE, "portal" );
	ResourceManager::loadTexture( "despawn_portal.png", GL_TRUE, "despawn_portal" );
	ResourceManager::loadTexture( "space.jpg", GL_TRUE, "game_background" );

	// tower sprites
	ResourceManager::loadTexture( "tower_true.png", GL_TRUE, "tower_true" );
	ResourceManager::loadTexture( "tower_true_void.png", GL_TRUE, "tower_true_void" );
	ResourceManager::loadTexture( "tower_true_plasma.png", GL_TRUE, "tower_true_plasma" );
	ResourceManager::loadTexture( "tower_true_ice.png", GL_TRUE, "tower_true_ice" );
	ResourceManager::loadTexture( "tower_void.png", GL_TRUE, "tower_void" );
	ResourceManager::loadTexture( "tower_void_true.png", GL_TRUE, "tower_void_true" );
	ResourceManager::loadTexture( "tower_void_plasma.png", GL_TRUE, "tower_void_plasma" );
	ResourceManager::loadTexture( "tower_void_ice.png", GL_TRUE, "tower_void_ice" );
	ResourceManager::loadTexture( "tower_plasma.png", GL_TRUE, "tower_plasma" );
	ResourceManager::loadTexture( "tower_plasma_true.png", GL_TRUE, "tower_plasma_true" );
	ResourceManager::loadTexture( "tower_plasma_void.png", GL_TRUE, "tower_plasma_void" );
	ResourceManager::loadTexture( "tower_plasma_ice.png", GL_TRUE, "tower_plasma_ice" );
	ResourceManager::loadTexture( "tower_ice.png", GL_TRUE, "tower_ice" );
	ResourceManager::loadTexture( "tower_ice_true.png", GL_TRUE, "tower_ice_true" );
	ResourceManager::loadTexture( "tower_ice_void.png", GL_TRUE, "tower_ice_void" );
	ResourceManager::loadTexture( "tower_ice_plasma.png", GL_TRUE, "tower_ice_plasma" );

	// bullet sprites
	ResourceManager::loadTexture( "ship_true_dmg_bullet.png", GL_TRUE, "ship_true_dmg_bullet" );
	ResourceManager::loadTexture( "bullet_true.png", GL_TRUE, "bullet_true" );
	ResourceManager::loadTexture( "bullet_true_void.png", GL_TRUE, "bullet_true_void" );
	ResourceManager::loadTexture( "bullet_true_plasma.png", GL_TRUE, "bullet_true_plasma" );
	ResourceManager::loadTexture( "bullet_true_ice.png", GL_TRUE, "bullet_true_ice" );
	ResourceManager::loadTexture( "bullet_void.png", GL_TRUE, "bullet_void" );
	ResourceManager::loadTexture( "bullet_void_true.png", GL_TRUE, "bullet_void_true" );
	ResourceManager::loadTexture( "bullet_void_plasma.png", GL_TRUE, "bullet_void_plasma" );
	ResourceManager::loadTexture( "bullet_void_ice.png", GL_TRUE, "bullet_void_ice" );
	ResourceManager::loadTexture( "bullet_ice.png", GL_TRUE, "bullet_ice" );
	ResourceManager::loadTexture( "bullet_ice_true.png", GL_TRUE, "bullet_ice_true" );
	ResourceManager::loadTexture( "bullet_ice_void.png", GL_TRUE, "bullet_ice_void" );
	ResourceManager::loadTexture( "bullet_ice_plasma.png", GL_TRUE, "bullet_ice_plasma" );
	ResourceManager::loadTexture( "bullet_plasma.png", GL_TRUE, "bullet_plasma" );
	ResourceManager::loadTexture( "bullet_plasma_true.png", GL_TRUE, "bullet_plasma_true" );
	ResourceManager::loadTexture( "bullet_plasma_void.png", GL_TRUE, "bullet_plasma_void" );
	ResourceManager::loadTexture( "bullet_plasma_ice.png", GL_TRUE, "bullet_plasma_ice" );

	initMenuButtons();

	// create systems
	systems.push_back( new HealthSystem( &numEnemiesLeft ) );
	systems.push_back( new RotationSystem( &numEnemiesLeft ) );
	systems.push_back( new PlayerInputSystem( &numEnemiesLeft ) );
	systems.push_back( new SlowedSystem( &numEnemiesLeft ) );
	systems.push_back( new FollowSystem( &numEnemiesLeft ) );
	systems.push_back( new MovementSystem( &numEnemiesLeft ) );
	shootSystem = new ShootSystem( &numEnemiesLeft );
	systems.push_back( shootSystem );
	path = new PathSystem( &numEnemiesLeft );
	systems.push_back( path );
	dmgAuraSystem = new DamageAuraSystem( &numEnemiesLeft );
	systems.push_back( dmgAuraSystem );
	systems.push_back( new SpawnSystem( &numEnemiesLeft ) );

	// initialize entity factory
	EntityFactory::setWorld( &world );
	EntityFactory::setCollisionSystem( &collisionSystem );
	EntityFactory::setShootSystem( shootSystem );
	EntityFactory::setDmgAuraSystem( dmgAuraSystem );

	init();
}


SpaceInvadersTD::~SpaceInvadersTD() {
	for ( int i = 0; i < systems.size(); ++i ) {
		delete systems[i];
	}
}

void SpaceInvadersTD::init() {
	// reset all systems
	for ( System*& system : systems ) {
		system->clear();
	}
	collisionSystem.clear();
	world.clear();

	// reset game attributes
	money = 100;
	numEnemiesLeft = 0;
	numLives = 100;
	selectedGridPos = glm::ivec2( -1 );
	tdState = TD_MENU;
	placeTowerMode = false;
	updateButtons = false;
	showButtons( true );

	// create grid
	gridSize = (float)GAME_WIDTH / NUM_GRID_COLS;
	grid.resize( NUM_GRID_ROWS );
	for ( int i = 0; i < NUM_GRID_ROWS; ++i ) {
		grid[i].resize( NUM_GRID_COLS );
		for ( int j = 0; j < NUM_GRID_COLS; ++j ) {
			grid[i][j].taken = false;
			grid[i][j].towerType = TOWER_NONE;
			grid[i][j].ent = -1;
			grid[i][j].pos = glm::vec2( gridSize * j, gridSize * i );
		}
	}

	// set game values
	currGridPulseTime = 0.0f;
	loadLevel( 1 );

	// create player
	EntityFactory::createPlayer();
}

void SpaceInvadersTD::updateSystem( System* system, int thread, int numThreads, float dt ) {
	for ( int i = thread; i < NUM_ENTITIES; i += numThreads ) {
		if ( system->condition( world.entities[i].mask ) ) {
			// update any entities that use the system
			system->update( &world, i, dt );
		}
	}
}

STATE SpaceInvadersTD::update( const float dt ) {

	if ( tdState == TD_EXIT ) {
		return MAIN_INIT;
	}

	currGridPulseTime += dt * 3.14;
	const int numThreads = 4;
	std::thread threads[numThreads];
	int entsPerThread = NUM_ENTITIES / numThreads;

	if ( tdState == TD_PLAYING ) {
		for ( auto& system : systems ) {

#if MULTITHREADED
			for ( int i = 0; i < numThreads; ++i ) {
				threads[i] = std::thread( &SpaceInvadersTD::updateSystem, this, system, i, numThreads, dt );
			}
			for ( int i = 0; i < numThreads; ++i ) { threads[i].join(); }
#else

			for ( int i = 0; i < NUM_ENTITIES; ++i ) {
				if ( system->condition( world.entities[i].mask ) ) {
					// update any entities that use the system
					system->update( &world, i, dt );
				}
			}
#endif
			// add any changes to entities caused by the system
			system->adjustEntityVector( &world );
			// reset the systems entity additiosn / removals
		}

		collisionSystem.update( &world );
		handleCollisionEvents();
		collisionSystem.clearCollisionEvents();

		// check to see if the game is over
		if ( numLives <= 0 ) {
			tdState = TD_LOSE;
			showButtons( true );
		} else if ( numEnemiesLeft <= 0 ) {
			if ( currRound == numRounds - 1) {
				tdState = TD_WIN;
			} else {
				tdState = TD_MENU;
				selectedGridPos = glm::ivec2( -1 );
				placeTowerMode = false;
			}
			showButtons( true );
		}

	} else if ( tdState == TD_MENU ) {
		if ( updateButtons ) {
			showButtons( true );
			updateButtons = false;
		}

		// handle place tower mode
		if ( placeTowerMode ) {
			if ( ServiceLocator::getInput().getKeyPressed( MOUSE_BUTTON_RIGHT ) && ServiceLocator::getInput().keyNotProcessed( MOUSE_BUTTON_RIGHT ) ) {
				ServiceLocator::getInput().setKeyProcessed( MOUSE_BUTTON_RIGHT );
				placeTowerMode = false;
			} else if ( ServiceLocator::getInput().getKeyPressed( MOUSE_BUTTON_LEFT ) ) {
				int gridX = ServiceLocator::getInput().getMousePos().x / gridSize;
				int gridY = ServiceLocator::getInput().getMousePos().y / gridSize;

				if ( ( gridX >= 0 && gridX < NUM_GRID_COLS ) && ( gridY >= 0 && gridY < NUM_GRID_ROWS ) && grid[gridY][gridX].ent == -1 ) {
					grid[gridY][gridX].taken = true;
					// test to see if path is not blocked
					WorldComponent& spawnWorld = world.worldComponents[world.getComponentIndex( spawners[0], WORLD )];
					WorldComponent& despawnWorld = world.worldComponents[world.getComponentIndex( despawners[0], WORLD )];
					SpawnComponent& spawnComp = world.spawnComponents[world.getComponentIndex( spawners[0], SPAWN )];
					if ( path->calcOptimalPath( ( glm::uvec2 ) ( spawnWorld.pos / gridSize ), ( glm::uvec2 ) ( despawnWorld.pos / gridSize ),
						spawnComp.spawnTypes[spawnComp.round + 1].getEntity()->world.size.x / 2.0f, grid ) ) {
						money -= 5;
						placeBaseTower( gridX, gridY );
						if ( !ServiceLocator::getInput().getKeyPressed( KEY_LEFT_SHIFT ) || money < 5 ) {
							placeTowerMode = false;
						}
					} else {
						grid[gridY][gridX].taken = false;
					}
				}
			}
			if ( ServiceLocator::getInput().getKeyPressed( KEY_T ) && ServiceLocator::getInput().keyNotProcessed( KEY_T ) ) {
				ServiceLocator::getInput().setKeyProcessed( KEY_T );
				placeTowerMode = false;
			}
		} else if ( ServiceLocator::getInput().getKeyPressed( KEY_T ) && ServiceLocator::getInput().keyNotProcessed( KEY_T ) && money >= 5 ) {
			ServiceLocator::getInput().setKeyProcessed( KEY_T );
			placeTowerMode = true;
		}

		if ( !placeTowerMode ) {
			if ( ServiceLocator::getInput().getKeyPressed( MOUSE_BUTTON_RIGHT ) && ServiceLocator::getInput().keyNotProcessed( MOUSE_BUTTON_RIGHT ) ) {
				ServiceLocator::getInput().setKeyProcessed( MOUSE_BUTTON_RIGHT );
				selectedGridPos = glm::ivec2( -1 );
				showButtons( true );
			}

			// handle selection mode
			if ( ServiceLocator::getInput().getKeyPressed( MOUSE_BUTTON_LEFT ) && ServiceLocator::getInput().keyNotProcessed( MOUSE_BUTTON_LEFT ) ) {
				ServiceLocator::getInput().setKeyProcessed( MOUSE_BUTTON_LEFT );
				int gridX = ServiceLocator::getInput().getMousePos().x / gridSize;
				int gridY = ServiceLocator::getInput().getMousePos().y / gridSize;
				if ( ( gridX >= 0 && gridX < NUM_GRID_COLS ) && ( gridY >= 0 && gridY < NUM_GRID_ROWS ) && grid[gridY][gridX].taken ) {
					// select the tower where the mouse is
					selectedGridPos = glm::ivec2( gridX, gridY );
					showButtons( true );
				}
			}
		}

		// start game is space pressed
		if ( ServiceLocator::getInput().getKeyPressed( KEY_SPACE ) ) {
			startRound();
		}
	}

	return GAME;
}

void SpaceInvadersTD::render() {
	// render background
	ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "game_background" ), glm::vec2( 0.0f ), glm::vec2( GAME_WIDTH, GAME_HEIGHT ), 0.0f );

	// render game grid lines
	// render horizontal lines
	for ( int i = 1; i <= NUM_GRID_ROWS; ++i ) {
		ServiceLocator::getGraphics().draw2DBox( glm::vec2( 0.0f, gridSize * i - 1 ), glm::vec2( gridSize * NUM_GRID_COLS, 2.0f ), GRID_COLOR + std::sin( currGridPulseTime / GRID_PULSE_TIME ) * GRID_PULSE_AMOUNT );
	}
	// render vertical lines
	for ( int i = 1; i < NUM_GRID_COLS; ++i ) {
		ServiceLocator::getGraphics().draw2DBox( glm::vec2( gridSize * i - 1, 0.0f ), glm::vec2( 2.0f, gridSize * NUM_GRID_ROWS ), GRID_COLOR + std::sin( currGridPulseTime / GRID_PULSE_TIME ) * GRID_PULSE_AMOUNT );
	}

	unsigned renderFlags = WORLD | RENDER;
	for ( int i = 0; i < NUM_ENTITIES; i++ ) {
		if ( !( ( renderFlags ^ world.entities[i].mask ) & renderFlags ) ) {
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( i, RENDER )];
			WorldComponent& worldComp = world.worldComponents[world.getComponentIndex( i, WORLD )];
			ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( renderComp.textureName ), worldComp.pos,
				worldComp.size, worldComp.rotation );
		}
	}

	// render health bars
	renderFlags = WORLD | HEALTH;
	for ( int i = 0; i < NUM_ENTITIES; i++ ) {
		if ( !( ( renderFlags ^ world.entities[i].mask ) & renderFlags ) ) {
			HealthComponent& healthComp = world.healthComponents[world.getComponentIndex( i, HEALTH )];
			WorldComponent& worldComp = world.worldComponents[world.getComponentIndex( i, WORLD )];
			ServiceLocator::getGraphics().draw2DBox( worldComp.pos - glm::vec2( 0.0f, worldComp.size.y * 0.2f ),
				glm::vec2( worldComp.size.x, worldComp.size.y * 0.2f ), glm::vec3( 1.0f, 0.0f, 1.0f ) );
			float padding = 1.0f;
			ServiceLocator::getGraphics().draw2DBox( worldComp.pos - glm::vec2( -padding, worldComp.size.y * 0.2f - padding ),
				glm::vec2( ( worldComp.size.x - ( 2.0f * padding ) ) * ( healthComp.currHP / healthComp.maxHP ), worldComp.size.y * 0.2f - ( 2.0f * padding ) ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		}
	}

	// draw gui background
	ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "hud" ), glm::vec2( 0.0f, GAME_HEIGHT * 0.75f ),
		glm::vec2( GAME_WIDTH, GAME_HEIGHT * 0.25f ), 0.0f );

	// render menu inbetween turns
	if ( tdState == TD_MENU ) {

		// get index mouse is over on the grid
		int gridX = ServiceLocator::getInput().getMousePos().x / gridSize;
		int gridY = ServiceLocator::getInput().getMousePos().y / gridSize;

		// indicate that game is in place tower mode
		if ( placeTowerMode ) {
			glm::vec2 pos;
			glm::vec3 color;
			if ( ( gridX >= 0 && gridX < NUM_GRID_COLS ) && ( gridY >= 0 && gridY < NUM_GRID_ROWS ) ) {
				pos = glm::vec2( gridX, gridY ) * gridSize;
				color = grid[gridY][gridX].ent != -1 ? glm::vec3( 1.0f, 0.0f, 0.0f ) : glm::vec3( 1.0f );
			} else {
				pos = ServiceLocator::getInput().getMousePos() - ( gridSize / 2.0f );
				color = glm::vec3( 1.0f );
			}
			ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "place_tower_indicator" ), pos,
				glm::vec2( gridSize ), 0.0f, glm::vec4( color, 1.0f ) );

		} else {
			// render grid hover
			if ( selectedGridPos != glm::ivec2( gridX, gridY ) && ( gridX >= 0 && gridX < NUM_GRID_COLS ) && ( gridY >= 0 && gridY < NUM_GRID_ROWS )
				&& grid[gridY][gridX].taken ) {
				glm::vec2 pos = glm::vec2( gridX, gridY ) * gridSize;
				ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "grid_hover" ), pos,
					glm::vec2( gridSize ), 0.0f, glm::vec4( 1.0f, 1.0f, 1.0f, 0.8f ) );
			}
			// render grid selected
			if ( towerIsSelected() ) {
				glm::vec2 pos = glm::vec2( selectedGridPos.x, selectedGridPos.y ) * gridSize;
				ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "grid_selected" ), pos,
					glm::vec2( gridSize ), 0.0f, glm::vec4( 1.0f, 1.0f, 1.0f, 0.8f ) );
			}
		}
	} else if ( tdState == TD_LOSE || tdState == TD_WIN ) {
		std::string gameMessage = tdState == TD_LOSE ? "Game Over: You Lose!" : "Victory!";
		glm::vec3 msgColor = tdState == TD_LOSE ? glm::vec3( 1.0f, 0.0f, 0.0f ) : glm::vec3( 0.0f, 1.0f, 0.0f );

		ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), gameMessage, glm::vec2( GAME_WIDTH, GAME_HEIGHT ) * 0.5f, 2.0f,
			msgColor, HOR_CENTERED, VERT_CENTERED);
	}

	// render buttons
	for ( Button* btn : buttons ) {
		btn->render( ServiceLocator::getGraphics() );
	}

	// render money
	std::stringstream ss;
	ss << "Round:    " << currRound + 1 << "/" << numRounds;
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), ss.str(), glm::vec2( 20, GAME_HEIGHT - 180 ), 1.0f );
	ss.str( std::string() );
	ss << "Money:    " << money;
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), ss.str(), glm::vec2( 20, GAME_HEIGHT - 140 ), 1.0f );
	// render enemies left
	ss.str( std::string() );
	ss << "Enemies : " << numEnemiesLeft;
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), ss.str(), glm::vec2( 20, GAME_HEIGHT - 100 ), 1.0f );
	// render enemies left
	ss.str( std::string() );
	ss << "Lives:    " << numLives;
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), ss.str(), glm::vec2( 20, GAME_HEIGHT - 60 ), 1.0f );
}

void SpaceInvadersTD::placeBaseTower( unsigned x, unsigned y ) {
	assert( x >= 0 && x < NUM_GRID_COLS && y >= 0 && y < NUM_GRID_ROWS );

	// create the tower
	int pos = EntityFactory::createBaseTower();
	WorldComponent& worldComp = world.worldComponents[world.getComponentIndex( pos, WORLD )];
	worldComp.size = glm::vec2( gridSize );
	worldComp.pos = glm::vec2( x * gridSize, y * gridSize );

	// mark that position as taken by a tower
	grid[y][x].taken = true;
	grid[y][x].towerType = TOWER_WALL;
	grid[y][x].ent = pos;
}

void SpaceInvadersTD::handleCollisionEvents() {
	for ( CollisionEvent event : collisionSystem.collisions ) {

		// make sure another event didn't already remove one of the entities
		if ( world.entities[event.ent1].mask == NONE || world.entities[event.ent2].mask == NONE ) {
			continue;
		}

		switch ( event.eventType ) {
		case DAMAGE_EVENT: {
			int receiver = ( world.entities[event.ent1].mask ) & HEALTH ? event.ent1 : event.ent2;
			int dealer = ( event.ent1 == receiver ) ? event.ent2 : event.ent1;
			DamageComponent& dmgComp = world.dmgComponents[world.getComponentIndex( dealer, DAMAGE )];
			HealthComponent& healthComp = world.healthComponents[world.getComponentIndex( receiver, HEALTH )];
			if ( healthComp.currHP > 0.0f ) {
				healthComp.takeDmg( dmgComp );

				// check to see if the entity died
				if ( healthComp.currHP <= 0.0f ) {
					if ( world.entities[receiver].mask & MONEY ) {
						money += world.moneyComponents[world.getComponentIndex( receiver, MONEY )].value;
					}
				} else {
					// slow entity if it can be slowed
					attemptToSlow( receiver, dealer, dmgComp.slowInfo );
				}
			}
			EntityFactory::removeEntity( dealer );
		}
		break;
		case DESPAWN_EVENT: {
			int ent = ( world.entities[event.ent1].mask ) & PATH ? event.ent1 : event.ent2;
			HealthComponent& healthComp = world.healthComponents[world.getComponentIndex( ent, HEALTH )];
			healthComp.currHP = 0.0f;
			numLives--;
		}
		break;
		case AOE_SLOW_EVENT: {
			// get enemy's position
			WorldComponent& enemyWorld = world.worldComponents[world.getComponentIndex( event.ent1, WORLD )];
			// get bullet's aoe info
			AOEComponent& aoeComp = world.AOEComponents[world.getComponentIndex( event.ent2, AOE )];
			// loop through nearby enemies and slow them
			for ( unsigned& ent : collisionSystem.registeredEnemies ) {
				WorldComponent& enemyWorld2 = world.worldComponents[world.getComponentIndex( ent, WORLD )];
				if ( glm::distance( enemyWorld.getCenter(), enemyWorld2.getCenter() ) <= aoeComp.range ) {
					// attempt to slow enemy
					attemptToSlow( ent, event.ent2, aoeComp.dmg.slowInfo);
				}
			}
		}
		case AOE_DAMAGE_EVENT: {
			// get enemy's position
			WorldComponent& enemyWorld = world.worldComponents[world.getComponentIndex( event.ent1, WORLD )];
			// get bullet's aoe info
			AOEComponent& aoeComp = world.AOEComponents[world.getComponentIndex( event.ent2, AOE )];
			// loop through nearby enemies and slow them
			for ( unsigned& ent : collisionSystem.registeredEnemies ) {
				WorldComponent& enemyWorld2 = world.worldComponents[world.getComponentIndex( ent, WORLD )];
				if ( glm::distance( enemyWorld.getCenter(), enemyWorld2.getCenter() ) <= aoeComp.range ) {
					// get enemy health
					HealthComponent& healthComp = world.healthComponents[world.getComponentIndex( ent, HEALTH )];
					if ( healthComp.currHP > 0.0f ) {
						healthComp.takeDmg( aoeComp.dmg );

						// check to see if the entity died
						if ( healthComp.currHP <= 0.0f ) {
							if ( world.entities[ent].mask & MONEY ) {
								money += world.moneyComponents[world.getComponentIndex( ent, MONEY )].value;
							}
						}
					}
				}
			}
		}
		break;
		}
	}
}

void SpaceInvadersTD::attemptToSlow( unsigned receiver, unsigned dealer, const SlowInfo& slowInfo ) {
	// slow entity if it can be slowed
	if ( slowInfo.type != SLOW_NONE && !( ( ( SLOWED | MOVEMENT ) ^ world.entities[receiver].mask ) & ( SLOWED | MOVEMENT ) ) ) {
		SlowedComponent& slowComp = world.slowComponents[world.getComponentIndex( receiver, SLOWED )];
		MovementComponent& moveComp = world.movementComponents[world.getComponentIndex( receiver, MOVEMENT )];

		if ( slowComp.canApplySlow( slowInfo.type, slowInfo.percentSpeed ) ) {

			// check to see if a special condition must be met
			bool applyingSlow = true;
			if ( world.entities[dealer].mask & PARENT ) {
				ParentComponent& parent = world.parentComponents[world.getComponentIndex( dealer, PARENT )];
				// if true ice tower make sure slow it hitting from behind
				if ( world.entities[parent.parentEnt].mask | SHOOT 
					&& world.shootComponents[world.getComponentIndex( parent.parentEnt, SHOOT )].towerType == TOWER_TRUE_ICE ) {
					// get bullet and enemy position
					WorldComponent& enemyWorld = world.worldComponents[world.getComponentIndex( receiver, WORLD )];
					WorldComponent& bulletWorld = world.worldComponents[world.getComponentIndex( dealer, WORLD )];
					glm::vec2 dirToBullet = bulletWorld.getCenter() - enemyWorld.getCenter();
					// only slow if behind the enemy
					if ( glm::dot( dirToBullet, moveComp.vel ) > 0 ) {
						applyingSlow = false;
					}
				}
			}

			if ( applyingSlow ) {
				bool alreadySlowed = false;
				for ( SlowInfo& info : slowComp.slowedInfo ) {
					if ( info.type == slowInfo.type ) {
						alreadySlowed = true;
						// reapply the new slow if it already existed
						moveComp.removeSlow( info.percentSpeed * info.speedAtApplication );
						info.timeLeft = slowInfo.timeLeft;
						info.percentSpeed = slowInfo.percentSpeed;
						moveComp.applySlow( slowInfo.percentSpeed * info.speedAtApplication );
						break;
					}
				}
				if ( !alreadySlowed ) {
					// slow the receiver based on a percent of their current speed
					slowComp.slowedInfo.push_back( SlowInfo( slowInfo.percentSpeed, slowInfo.timeLeft, moveComp.getCurrSpeed() / moveComp.defSpeed, slowInfo.type ) );
					moveComp.applySlow( slowInfo.percentSpeed * moveComp.getCurrSpeed() / moveComp.defSpeed );
				}
			}
		}
	}
}

bool SpaceInvadersTD::towerIsSelected() const {
	return selectedGridPos != glm::ivec2( -1 );
}

void SpaceInvadersTD::showButtons( bool show ) {
	for ( Button*& btn : buttons ) {
		btn->setVisible( false );
	}
	if ( show && tdState != TD_LOSE && tdState != TD_WIN && show ) {
		bStartRound.setVisible( true );
		if ( !placeTowerMode && !towerIsSelected() ) {
			// show place wall button if no tower is selected and not in place tower mode
			bPlaceWall.setVisible( true );
		} else if ( !placeTowerMode && towerIsSelected() ) {
			// show button based on the type of tower that is selected
			for ( Button* btn : towerButtons[grid[selectedGridPos.y][selectedGridPos.x].towerType] ) {
				btn->setVisible( true );
			}
			bSellTower.setVisible( true );
		}
	}
	bMainMenu.setVisible( true );
}

void SpaceInvadersTD::startRound() {
	SpawnComponent& spawn = world.spawnComponents[world.getComponentIndex( spawners[0], SPAWN )];
	if ( spawn.round < (int)spawn.numRounds ) {
		WorldComponent& spawnWorld = world.worldComponents[world.getComponentIndex( spawners[0], WORLD )];
		WorldComponent& despawnWorld = world.worldComponents[world.getComponentIndex( despawners[0], WORLD )];
		SpawnComponent& spawnComp = world.spawnComponents[world.getComponentIndex( spawners[0], SPAWN )];
		currRound++;
		spawn.round = currRound;
		spawn.currSpawnNum = 0;
		numEnemiesLeft = spawn.spawnTypes[spawn.round].num;
		tdState = TD_PLAYING;
		placeTowerMode = false;
		path->calcOptimalPath( ( glm::uvec2 ) ( spawnWorld.pos / gridSize ), ( glm::uvec2 ) ( despawnWorld.pos / gridSize ),
			spawnComp.spawnTypes[spawnComp.round].getEntity()->world.size.x / 2.0f, grid );
		updateButtons = true;
		showButtons( false );
	}
}

void SpaceInvadersTD::initMenuButtons() {
	// start round button
	bStartRound.setSize( glm::vec2( 256, 96 ) );
	bStartRound.setPos( glm::vec2( ( GAME_WIDTH * 0.5f ) - ( bStartRound.getSize().x * 0.5f ),
		( GAME_HEIGHT * 0.9f ) - ( bStartRound.getSize().y * 0.5f ) ) );
	bStartRound.setText( "Start Round" );
	bStartRound.setOnClickFunction( [&]() {
		startRound();
	} );
	buttons.push_back( &bStartRound );

	// main menu button
	bMainMenu.setSize( glm::vec2( 192, 64 ) );
	bMainMenu.setPos( glm::vec2( 20, GAME_HEIGHT - 256) );
	bMainMenu.setText( "Main Menu" );
	bMainMenu.setOnClickFunction( [&] () {
		tdState = TD_EXIT;
	} );
	buttons.push_back( &bMainMenu );

	// sell tower button
	bSellTower.setSize( glm::vec2( 100, 100 ) );
	bSellTower.setPos( glm::vec2( 1920 - 480 + 24 + 3 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 + 100 + 22 ) );
	bSellTower.setText( "Sell" );
	bSellTower.setOnClickFunction( [&]() {
		// sell the currently selected tower
		if ( towerIsSelected() ) {
			money += world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value;
			EntityFactory::removeEntity( grid[selectedGridPos.y][selectedGridPos.x].ent );
			grid[selectedGridPos.y][selectedGridPos.x].ent = -1;
			grid[selectedGridPos.y][selectedGridPos.x].taken = false;
			selectedGridPos = glm::ivec2( -1 );
			updateButtons = true;
		}
	} );
	buttons.push_back( &bSellTower );

	// place wall button
	bPlaceWall.setSize( glm::vec2( 100, 100 ) );
	bPlaceWall.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bPlaceWall.setText( "Wall" );
	bPlaceWall.setOnClickFunction( [&]() {
		if ( money >= 5 && !placeTowerMode ) {
			placeTowerMode = true;
		}
	} );
	buttons.push_back( &bPlaceWall );

	// upgrade true button
	bUpgradeTrue.setSize( glm::vec2( 100, 100 ) );
	bUpgradeTrue.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bUpgradeTrue.setText( "True" );
	bUpgradeTrue.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			int shootIndex = EntityFactory::addComponent( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT );
			ShootComponent& shootComp = world.shootComponents[shootIndex];
			shootComp.attackSpeed = 0.5f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 10.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 1000.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 300.0f;
			shootComp.bulletTexture = "bullet_true";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_true";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_TRUE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bUpgradeTrue );

	// upgrade void button
	bUpgradeVoid.setSize( glm::vec2( 100, 100 ) );
	bUpgradeVoid.setPos( glm::vec2( 1920 - 480 + 24 + 100 + ( 32 / 3 ), GAME_HEIGHT * 0.75f + 24 ) );
	bUpgradeVoid.setText( "Void" );
	bUpgradeVoid.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			int shootIndex = EntityFactory::addComponent( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT );
			ShootComponent& shootComp = world.shootComponents[shootIndex];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 30.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 500.0f;
			shootComp.bulletTexture = "bullet_void";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_void";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_VOID;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bUpgradeVoid );

	// upgrade plasma button
	bUpgradePlasma.setSize( glm::vec2( 100, 100 ) );
	bUpgradePlasma.setPos( glm::vec2( 1920 - 480 + 24 + 2 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bUpgradePlasma.setText( "Plasma" );
	bUpgradePlasma.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			int shootIndex = EntityFactory::addComponent( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT );
			ShootComponent& shootComp = world.shootComponents[shootIndex];
			shootComp.attackSpeed = 0.25f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 10.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 800.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 300.0f;
			shootComp.bulletTexture = "bullet_plasma";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_plasma";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_PLASMA;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bUpgradePlasma );

	// upgrade ice button
	bUpgradeIce.setSize( glm::vec2( 100, 100 ) );
	bUpgradeIce.setPos( glm::vec2( 1920 - 480 + 24 + 3 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bUpgradeIce.setText( "Ice" );
	bUpgradeIce.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			int shootIndex = EntityFactory::addComponent( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT );
			ShootComponent& shootComp = world.shootComponents[shootIndex];
			shootComp.attackSpeed = 0.5f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 30.0f;
			shootComp.bulletDmg.slowInfo.percentSpeed = 0.25f;
			shootComp.bulletDmg.slowInfo.timeLeft = 2.0f;
			shootComp.bulletDmg.slowInfo.type = SLOW_ICE;
			shootComp.bulletSpeed = 300.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 600.0f;
			shootComp.bulletTexture = "bullet_ice";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_ice";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_ICE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bUpgradeIce );

	// upgrade true void button
	bTrueVoid.setSize( glm::vec2( 100, 100 ) );
	bTrueVoid.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bTrueVoid.setText( "TV" );
	bTrueVoid.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 40.0f;
			shootComp.bulletDmg.voidDmg = 10.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 600.0f;
			shootComp.bulletTexture = "bullet_true_void";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_true_void";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_TRUE_VOID;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bTrueVoid );

	// upgrade true plasma button
	bTruePlasma.setSize( glm::vec2( 100, 100 ) );
	bTruePlasma.setPos( glm::vec2( 1920 - 480 + 24 + 1 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bTruePlasma.setText( "TP" );
	bTruePlasma.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 0.35f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 20.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 800.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 400.0f;
			shootComp.bulletTexture = "bullet_true_plasma";
			shootComp.towerType = TOWER_TRUE_PLASMA;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_true_plasma";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_TRUE_PLASMA;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bTruePlasma );

	// upgrade true ice button
	bTrueIce.setSize( glm::vec2( 100, 100 ) );
	bTrueIce.setPos( glm::vec2( 1920 - 480 + 24 + 2 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bTrueIce.setText( "TI" );
	bTrueIce.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 0.75f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 30.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 10.0f;
			shootComp.bulletDmg.slowInfo.percentSpeed = 0.4f;
			shootComp.bulletDmg.slowInfo.timeLeft = 1.0f;
			shootComp.bulletDmg.slowInfo.type = SLOW_TRUE_ICE;
			shootComp.bulletSpeed = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 750.0f;
			shootComp.bulletTexture = "bullet_true_ice";
			shootComp.towerType = TOWER_TRUE_ICE;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_true_ice";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_TRUE_PLASMA;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bTrueIce );

	// upgrade void true button
	bVoidTrue.setSize( glm::vec2( 100, 100 ) );
	bVoidTrue.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bVoidTrue.setText( "VT" );
	bVoidTrue.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 40.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 300.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 500.0f;
			shootComp.bulletTexture = "bullet_void_true";
			shootComp.towerType = TOWER_VOID_TRUE;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_void_true";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_VOID_TRUE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bVoidTrue );

	// upgrade void true button
	bVoidPlasma.setSize( glm::vec2( 100, 100 ) );
	bVoidPlasma.setPos( glm::vec2( 1920 - 480 + 24 + 1 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bVoidPlasma.setText( "VP" );
	bVoidPlasma.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 40.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_void_plasma";
			shootComp.towerType = TOWER_VOID_PLASMA;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_void_plasma";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_VOID_PLASMA;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bVoidPlasma );

	// upgrade void true button
	bVoidIce.setSize( glm::vec2( 100, 100 ) );
	bVoidIce.setPos( glm::vec2( 1920 - 480 + 24 + 2 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bVoidIce.setText( "VI" );
	bVoidIce.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 40.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 15.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_void_ice";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_void_ice";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_VOID_ICE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bVoidIce );

	// upgrade void true button
	bPlasmaTrue.setSize( glm::vec2( 100, 100 ) );
	bPlasmaTrue.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bPlasmaTrue.setText( "PT" );
	bPlasmaTrue.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 10.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 40.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_plasma_true";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_plasma_true";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_PLASMA_TRUE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bPlasmaTrue );

	// upgrade void true button
	bPlasmaVoid.setSize( glm::vec2( 100, 100 ) );
	bPlasmaVoid.setPos( glm::vec2( 1920 - 480 + 24 + 1 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bPlasmaVoid.setText( "PV" );
	bPlasmaVoid.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 15.0f;
			shootComp.bulletDmg.plasmaDmg = 40.0f;
			shootComp.bulletDmg.iceDmg = 0.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_plasma_void";
			shootComp.towerType = TOWER_PLASMA_VOID;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_plasma_void";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			// give the tower a damage aura
			int auraIndex = EntityFactory::addComponent( grid[selectedGridPos.y][selectedGridPos.x].ent, DAMAGE_AURA );
			DmgAuraComponent& auraComp = world.dmgAuraComponents[auraIndex];
			auraComp.dmg.voidDmg = 3.0f;
			auraComp.pulseSpeed = 0.5f;
			auraComp.range = 150.0f;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_PLASMA_VOID;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bPlasmaVoid );

	// upgrade void true button
	bPlasmaIce.setSize( glm::vec2( 100, 100 ) );
	bPlasmaIce.setPos( glm::vec2( 1920 - 480 + 24 + 2 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bPlasmaIce.setText( "PI" );
	bPlasmaIce.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 40.0f;
			shootComp.bulletDmg.iceDmg = 15.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_plasma_ice";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_plasma_ice";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_PLASMA_ICE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bPlasmaIce );

	// upgrade void true button
	bIceTrue.setSize( glm::vec2( 100, 100 ) );
	bIceTrue.setPos( glm::vec2( 1920 - 480 + 24, GAME_HEIGHT * 0.75f + 24 ) );
	bIceTrue.setText( "IT" );
	bIceTrue.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 10.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 40.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_ice_true";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_ice_true";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_ICE_TRUE;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bIceTrue );

	// upgrade void true button
	bIceVoid.setSize( glm::vec2( 100, 100 ) );
	bIceVoid.setPos( glm::vec2( 1920 - 480 + 24 + 1 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bIceVoid.setText( "IV" );
	bIceVoid.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 0.0f;
			shootComp.bulletDmg.iceDmg = 40.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_ice_void";
			shootComp.towerType = TOWER_ICE_VOID;
			shootComp.bulletDmg.slowInfo.percentSpeed = 0.25f;
			shootComp.bulletDmg.slowInfo.timeLeft = 1.0f;
			shootComp.bulletDmg.slowInfo.type = SLOW_ICE_VOID;
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_ice_void";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_ICE_VOID;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bIceVoid );

	// upgrade void true button
	bIcePlasma.setSize( glm::vec2( 100, 100 ) );
	bIcePlasma.setPos( glm::vec2( 1920 - 480 + 24 + 2 * ( 100 + ( 32 / 3 ) ), GAME_HEIGHT * 0.75f + 24 ) );
	bIcePlasma.setText( "IP" );
	bIcePlasma.setOnClickFunction( [&]() {
		// add shoot component to wall
		if ( money >= 10 ) {
			money -= 10;
			ShootComponent& shootComp = world.shootComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, SHOOT )];
			shootComp.attackSpeed = 1.0f;
			shootComp.timePassed = shootComp.attackSpeed;
			shootComp.bulletDmg.trueDmg = 0.0f;
			shootComp.bulletDmg.voidDmg = 0.0f;
			shootComp.bulletDmg.plasmaDmg = 15.0f;
			shootComp.bulletDmg.iceDmg = 40.0f;
			shootComp.bulletSpeed = 500.0f;
			shootComp.range = 500.0f;
			shootComp.bulletSize = 0.25f;
			shootComp.bulletTexture = "bullet_ice_plasma";
			RenderComponent& renderComp = world.renderComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, RENDER )];
			renderComp.textureName = "tower_ice_plasma";
			world.moneyComponents[world.getComponentIndex( grid[selectedGridPos.y][selectedGridPos.x].ent, MONEY )].value += 5;
			grid[selectedGridPos.y][selectedGridPos.x].towerType = TOWER_ICE_PLASMA;
			updateButtons = true;
		}
	} );
	buttons.push_back( &bIcePlasma );

	// set which towers show which buttons
	towerButtons[TOWER_WALL].push_back( &bUpgradeTrue );
	towerButtons[TOWER_WALL].push_back( &bUpgradeVoid );
	towerButtons[TOWER_WALL].push_back( &bUpgradePlasma );
	towerButtons[TOWER_WALL].push_back( &bUpgradeIce );

	towerButtons[TOWER_TRUE].push_back( &bTrueVoid );
	towerButtons[TOWER_TRUE].push_back( &bTruePlasma );
	towerButtons[TOWER_TRUE].push_back( &bTrueIce );

	towerButtons[TOWER_VOID].push_back( &bVoidTrue );
	towerButtons[TOWER_VOID].push_back( &bVoidPlasma );
	towerButtons[TOWER_VOID].push_back( &bVoidIce );

	towerButtons[TOWER_PLASMA].push_back( &bPlasmaTrue );
	towerButtons[TOWER_PLASMA].push_back( &bPlasmaVoid );
	towerButtons[TOWER_PLASMA].push_back( &bPlasmaIce );

	towerButtons[TOWER_ICE].push_back( &bIceTrue );
	towerButtons[TOWER_ICE].push_back( &bIceVoid );
	towerButtons[TOWER_ICE].push_back( &bIcePlasma );

	// register buttons
	for ( Button*& btn : buttons ) {
		ServiceLocator::getInput().addOnClickObserver( btn );
	}
}

void SpaceInvadersTD::loadLevel( int level ) {
	std::stringstream ss;
	ss << "resources/levels/" << "level_" << level << ".txt";
	std::ifstream fin( ss.str() );

	// load spawn info for the level
	if ( fin.good() ) {
		// create spawner
		int spawner = EntityFactory::createSpawner();
		spawners.push_back( spawner );
		WorldComponent& worldComp = world.worldComponents[world.getComponentIndex( spawner, WORLD )];
		SpawnComponent& spawnComp = world.spawnComponents[world.getComponentIndex( spawner, SPAWN )];

		// get number of rounds for the level
		fin >> numRounds;
		currRound = -1;
		spawnComp.numRounds = numRounds;
		spawnComp.spawnTypes.resize( numRounds );

		// load start and end pos for portal
		glm::vec2 start, end;
		fin >> start.x >> start.y >> end.x >> end.y;

		worldComp.size = glm::vec2( gridSize );
		worldComp.pos = start * gridSize;
		grid[worldComp.pos.y / gridSize][worldComp.pos.x / gridSize].ent = spawner;

		// create a despawn portal
		int despawner = EntityFactory::createEntity( WORLD | RENDER | COLLISION | ROTATION );
		WorldComponent& despWorld = world.worldComponents[world.getComponentIndex( despawner, WORLD )];
		RenderComponent& despRender = world.renderComponents[world.getComponentIndex( despawner, RENDER )];
		CollisionComponent& despCollision = world.collisionComponents[world.getComponentIndex( despawner, COLLISION )];
		world.rotationComponents[world.getComponentIndex( despawner, ROTATION )].rotSpeed = 90.0f;
		despawners.push_back( despawner );

		// init despawn world data
		despWorld.size = glm::vec2( gridSize );
		despWorld.pos = end * gridSize;
		grid[despWorld.pos.y / gridSize][despWorld.pos.x / gridSize].ent = despawner;
		// init despawn render data
		despRender.textureName = "despawn_portal";
		// init collision data
		despCollision.shape = CIRCLE;
		despCollision.collisionID = DESPAWN;
		despCollision.collisionMask = ENEMY;
		despCollision.collisionScale = 0.01f;

		// loop over info for each round
		int i = 0;
		while ( fin.good() && i < numRounds ) {
			int numSpawns;
			float spawnRate;

			// load number of spawns and spawn rate
			fin >> numSpawns >> spawnRate;
			spawnComp.spawnTypes[i].num = numSpawns;
			spawnComp.spawnTypes[i].spawnRate = spawnRate;

			// create entity for the level
			Entity enemyType( HEALTH | RENDER | WORLD | MOVEMENT | PATH | COLLISION | MONEY | SLOWED );

			// load component info for entity
			char tag[20], subTag[20];
			bool loading = true;
			while ( loading ) {
				fin >> tag;
				if ( strcmp( tag, "HEALTH" ) == 0 ) {
					fin >> subTag >> enemyType.health.maxHP;
					fin >> subTag >> enemyType.health.voidArmor;
					fin >> subTag >> enemyType.health.plasmaArmor;
					fin >> subTag >> enemyType.health.iceArmor;
					enemyType.health.currHP = enemyType.health.maxHP;
				} else if ( strcmp( tag, "RENDER" ) == 0 ) {
					fin >> subTag >> enemyType.render.textureName;
					enemyType.render.color = glm::vec3( 1.0f );
				} else if ( strcmp( tag, "WORLD" ) == 0 ) {
					fin >> subTag;
					float size = 0.0f;
					assert( size <= 1.0f && "Size must be 1.0f or less " );
					fin >> size;
					enemyType.world.size = glm::vec2( size * gridSize );
					enemyType.world.pos = glm::vec2( worldComp.pos + ( worldComp.size / 2.0f ) - ( enemyType.world.size / 2.0f ) );
					enemyType.world.rotation = 0.0f;
				} else if ( strcmp( tag, "MOVEMENT" ) == 0 ) {
					fin >> subTag;
					float speed = 1.0f;
					fin >> speed;
					enemyType.movement.defSpeed = speed * gridSize;
					enemyType.movement.vel = glm::vec2( enemyType.movement.defSpeed, 0.0f );
				} else if ( strcmp( tag, "COLLISION" ) == 0 ) {
					char shape[20];
					fin >> subTag >> shape;
					if ( strcmp( shape, "CIRCLE" ) == 0 ) {
						enemyType.collision.shape = CIRCLE;
					} else {
						enemyType.collision.shape = RECTANGLE;
					}
					enemyType.collision.collisionID = ENEMY;
					enemyType.collision.collisionMask = BULLET | DESPAWN;
				} else if ( strcmp( tag, "MONEY" ) == 0 ) {
					fin >> subTag >> enemyType.money.value;
				} else {
					loading = false;
				}
			}

			// set the entity to spawn for the round
			spawnComp.spawnTypes[i].setEntity( &enemyType );
			++i;
		}

		// calcuate path for spawned enemies
		path->calcOptimalPath( ( glm::uvec2 ) start, ( glm::uvec2 ) end, worldComp.size.x, grid );

	} else {
		std::cout << "failed to load level" << std::endl;
	}
}