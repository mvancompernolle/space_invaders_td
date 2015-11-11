#include "EntityFactory.h"
#include "Consts.h"

World* EntityFactory::world;
CollisionSystem* EntityFactory::collisionSystem;
ShootSystem* EntityFactory::shootSystem;

EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

void EntityFactory::setWorld( World* w ) {
	world = w;
}

void EntityFactory::setCollisionSystem( CollisionSystem* cs ) {
	collisionSystem = cs;
}

void EntityFactory::setShootSystem( ShootSystem* ss ) {
	shootSystem = ss;
}

void EntityFactory::removeEntity( int pos ) {
	for ( int i = 1; i < COMPONENT_SIZE; i <<= 1 ) {
		switch ( i & world->entities[pos].mask ) {
		case HEALTH:
			world->healthComponents.remove( world->getComponentIndex( pos, HEALTH ) );
			break;
		case WORLD:
			world->worldComponents.remove( world->getComponentIndex( pos, WORLD ) );
			break;
		case RENDER:
			world->renderComponents.remove( world->getComponentIndex( pos, RENDER ) );
			break;
		case MOVEMENT:
			world->movementComponents.remove( world->getComponentIndex( pos, MOVEMENT ) );
			break;
		case PATH:
			world->pathComponents.remove( world->getComponentIndex( pos, PATH ) );
			break;
		case SPAWN:
			world->spawnComponents.remove( world->getComponentIndex( pos, SPAWN ) );
			break;
		case COLLISION:
			world->collisionComponents.remove( world->getComponentIndex( pos, COLLISION ) );
			collisionSystem->unregisterEntity( pos );
			// remove entity from shoot system if an enemy
			shootSystem->unregisterEntity( pos );
			break;
		case PLAYER_INPUT:
			world->playerInputComponents.remove( world->getComponentIndex( pos, PLAYER_INPUT ) );
			break;
		case DAMAGE:
			world->dmgComponents.remove( world->getComponentIndex( pos, DAMAGE ) );
			break;
		case MONEY:
			world->moneyComponents.remove( world->getComponentIndex( pos, MONEY ) );
			break;
		case SHOOT:
			world->shootComponents.remove( world->getComponentIndex( pos, SHOOT ) );
			break;
		case FOLLOW:
			world->followComponents.remove( world->getComponentIndex( pos, FOLLOW ) );
			break;
		}
	}
	world->entities[pos].mask = NONE;
	world->entities.remove( pos );
}

int EntityFactory::createEntity( unsigned mask ) {
	int index = world->entities.create();
	world->entities[index].mask = mask;
	if ( mask & WORLD ) { world->entities[index].componentIndices[WORLD] = world->worldComponents.create(); }
	if ( mask & HEALTH ) { world->entities[index].componentIndices[HEALTH] = world->healthComponents.create(); }
	if ( mask & RENDER ) { world->entities[index].componentIndices[RENDER] = world->renderComponents.create(); }
	if ( mask & PATH ) { world->entities[index].componentIndices[PATH] = world->pathComponents.create(); }
	if ( mask & MOVEMENT ) { world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create(); }
	if ( mask & SPAWN ) { world->entities[index].componentIndices[SPAWN] = world->spawnComponents.create(); }
	if ( mask & PLAYER_INPUT ) { world->entities[index].componentIndices[PLAYER_INPUT] = world->playerInputComponents.create(); }
	if ( mask & COLLISION ) { 
		world->entities[index].componentIndices[COLLISION] = world->collisionComponents.create(); 
		collisionSystem->registerEntity( index );
	}
	if ( mask & DAMAGE ) { world->entities[index].componentIndices[DAMAGE] = world->dmgComponents.create(); }
	if ( mask & MONEY ) { world->entities[index].componentIndices[MONEY] = world->moneyComponents.create(); }
	if ( mask & SHOOT ) { world->entities[index].componentIndices[SHOOT] = world->shootComponents.create(); }
	if ( mask & FOLLOW ) { world->entities[index].componentIndices[FOLLOW] = world->followComponents.create(); }
	return index;
}

int EntityFactory::createEnemy() {
	int index = world->entities.create();
	world->entities[index].mask = ( HEALTH | RENDER | WORLD | MOVEMENT | PATH | COLLISION );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[HEALTH] = world->healthComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create(); 
	world->entities[index].componentIndices[PATH] = world->pathComponents.create(); 
	world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create(); 

	world->entities[index].componentIndices[COLLISION] = world->collisionComponents.create();
	collisionSystem->registerEntity( index );

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->entities[index].componentIndices[WORLD]];
	worldComp.pos = glm::vec2( 0 );
	worldComp.size = glm::vec2( 32 );
	MovementComponent& movComp = world->movementComponents[world->entities[index].componentIndices[MOVEMENT]];
	movComp.vel = glm::vec2( 200.0f, 0.0f );
	movComp.defSpeed = 200.0f;
	RenderComponent& renderComp = world->renderComponents[world->entities[index].componentIndices[RENDER]];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "enemy";
	HealthComponent& healthComp = world->healthComponents[world->entities[index].componentIndices[HEALTH]];
	healthComp.currHP = 100.0f;
	healthComp.maxHP = 100.0f;
	CollisionComponent& collComp = world->collisionComponents[world->entities[index].componentIndices[COLLISION]];
	collComp.shape = CIRCLE;
	collComp.collisionID = ENEMY;
	collComp.collisionMask = BULLET;

	return index;
}

int EntityFactory::createBaseTower() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | SHOOT );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create();
	world->entities[index].componentIndices[SHOOT] = world->shootComponents.create();

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->entities[index].componentIndices[WORLD]];
	worldComp.rotation = 0.0f;
	worldComp.size = glm::vec2( 64 );
	RenderComponent& renderComp = world->renderComponents[world->entities[index].componentIndices[RENDER]];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "tower_base";

	return index;
}

int EntityFactory::createPlayer() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | MOVEMENT | PLAYER_INPUT | SHOOT );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create();
	world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create();
	world->entities[index].componentIndices[PLAYER_INPUT] = world->playerInputComponents.create();
	world->entities[index].componentIndices[SHOOT] = world->shootComponents.create();

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.rotation = 0.0f;
	worldComp.size = glm::vec2( 32 );
	worldComp.pos = glm::vec2( GAME_WIDTH * 0.5f - worldComp.size.x / 2.0f, GAME_HEIGHT * 0.7f );
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "enemy";
	MovementComponent& movComp = world->movementComponents[world->entities[index].componentIndices[MOVEMENT]];
	movComp.vel = glm::vec2( 0.0f, 0.0f );
	movComp.defSpeed = 200.0f;

	return index;
}

int EntityFactory::createSpawner() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | SPAWN );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create();
	world->entities[index].componentIndices[SPAWN] = world->spawnComponents.create();

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.rotation = 0.0f;
	worldComp.size = glm::vec2( 32 );
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "portal";
	SpawnComponent& spawnComp = world->spawnComponents[world->getComponentIndex( index, SPAWN )];
	spawnComp.round = -1;
	spawnComp.numRounds = 20;

	return index;
}

void EntityFactory::addEntity( Entity ent ) {
	int entPos = EntityFactory::createEntity( ent.componentTypes );
	for ( int j = 1; j < COMPONENT_SIZE; j <<= 1 ) {
		switch ( j & ent.componentTypes ) {
		case HEALTH:
			world->healthComponents[world->getComponentIndex( entPos, HEALTH )] = ent.health;
			break;
		case WORLD:
			world->worldComponents[world->getComponentIndex( entPos, WORLD )] = ent.world;
			break;
		case RENDER:
			world->renderComponents[world->getComponentIndex( entPos, RENDER )] = ent.render;
			break;
		case MOVEMENT:
			world->movementComponents[world->getComponentIndex( entPos, MOVEMENT )] = ent.movement;
			break;
		case PATH:
			world->pathComponents[world->getComponentIndex( entPos, PATH )] = ent.path;
			break;
		case SPAWN:
			world->spawnComponents[world->getComponentIndex( entPos, SPAWN )] = ent.spawn;
			break;
		case COLLISION:
			world->collisionComponents[world->getComponentIndex( entPos, COLLISION )] = ent.collision;
			if ( ent.collision.collisionID == ENEMY ) {
				shootSystem->registerEntity( entPos );
			}
			break;
		case PLAYER_INPUT:
			world->playerInputComponents[world->getComponentIndex( entPos, PLAYER_INPUT )] = ent.playerInput;
			break;
		case DAMAGE:
			world->dmgComponents[world->getComponentIndex( entPos, DAMAGE )] = ent.damage;
			break;
		case MONEY:
			world->moneyComponents[world->getComponentIndex( entPos, MONEY )] = ent.money;
			break;
		case SHOOT:
			world->shootComponents[world->getComponentIndex( entPos, SHOOT )] = ent.shoot;
			break;
		case FOLLOW:
			world->followComponents[world->getComponentIndex( entPos, FOLLOW )] = ent.follow;
			break;
		}
	}
}