#include "EntityFactory.h"
#include "Consts.h"

World* EntityFactory::world;
CollisionSystem* EntityFactory::collisionSystem;

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
	world->entities[index].mask = ( RENDER | WORLD );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create();

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
	world->entities[index].mask = ( RENDER | WORLD | MOVEMENT | PLAYER_INPUT );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create();
	world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create();
	world->entities[index].componentIndices[PLAYER_INPUT] = world->playerInputComponents.create();

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
	spawnComp.numRounds = 20;

	return index;
}
