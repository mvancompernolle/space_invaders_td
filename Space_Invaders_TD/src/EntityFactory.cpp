#include "EntityFactory.h"



EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

int EntityFactory::createEntity( World* world, unsigned mask ) {
	int index = world->entities.create();
	world->entities[index].mask = mask;
	if ( mask | WORLD ) { world->entities[index].componentIndices[WORLD] = world->worldComponents.create(); }
	if ( mask | HEALTH ) { world->entities[index].componentIndices[HEALTH] = world->healthComponents.create(); }
	if ( mask | RENDER ) { world->entities[index].componentIndices[RENDER] = world->renderComponents.create(); }
	if ( mask | PATH ) { world->entities[index].componentIndices[PATH] = world->pathComponents.create(); }
	if ( mask | MOVEMENT ) { world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create(); }
	if ( mask | SPAWN ) { world->entities[index].componentIndices[SPAWN] = world->spawnComponents.create(); }
	return index;
}

int EntityFactory::createEnemy( World* world ) {
	int index = world->entities.create();
	world->entities[index].mask = ( HEALTH | RENDER | WORLD | MOVEMENT | PATH );
	world->entities[index].componentIndices[WORLD] = world->worldComponents.create();
	world->entities[index].componentIndices[HEALTH] = world->healthComponents.create();
	world->entities[index].componentIndices[RENDER] = world->renderComponents.create(); 
	world->entities[index].componentIndices[PATH] = world->pathComponents.create(); 
	world->entities[index].componentIndices[MOVEMENT] = world->movementComponents.create(); 

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->entities[index].componentIndices[WORLD]];
	worldComp.pos = glm::vec2( 0 );
	worldComp.size = glm::vec2( 32 );
	MovementComponent& movComp = world->movementComponents[world->entities[index].componentIndices[MOVEMENT]];
	movComp.vel = glm::vec2( 250.0f, 0.0f );
	movComp.defSpeed = 250.0f;
	RenderComponent& renderComp = world->renderComponents[world->entities[index].componentIndices[RENDER]];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "enemy";
	HealthComponent& healthComp = world->healthComponents[world->entities[index].componentIndices[HEALTH]];
	healthComp.currHP = 100.0f;
	healthComp.maxHP = 100.0f;

	return index;
}

int EntityFactory::createBaseTower( World* world ) {
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

int EntityFactory::createPlayer( World* world ) {
	return 0;
}

int EntityFactory::createSpawner( World* world ) {
	return 0;
}


/*
Entity EntityFactory::createEnemy( World* world ) {
Entity ent( HEALTH | RENDER | WORLD | MOVEMENT | PATH );

// init world data
ent.world->pos = glm::vec2( 0 );
ent.world->size = glm::vec2( 32 );
// init health data
ent.health->currHP = 100.0f;
ent.health->maxHP = 100.0f;
// init movement data
ent.movement->vel = glm::vec2( 250.0f, 0.0f );
ent.movement->defSpeed = 250.0f;
// init render data
ent.render->color = glm::vec3( 1.0f );
ent.render->textureName = "enemy";
// init ai data
ent.path->target = glm::vec2( 0.0f, 0.0f );
return ent;
}

Entity EntityFactory::createBaseTower() {
Entity ent( RENDER | WORLD );

// init world data
ent.world->size = glm::vec2( 64 );
ent.world->rotation = 0.0f;
// init render data
ent.render->color = glm::vec3( 1.0f );
ent.render->textureName = "tower_base";

return ent;
}

Entity EntityFactory::createPlayer() {
Entity ent( RENDER | WORLD | MOVEMENT | KEYBOARD );
return ent;
}

Entity EntityFactory::createSpawner() {
Entity ent( RENDER | WORLD | SPAWN );

// init world data
ent.world->pos = glm::vec2( 0 );
ent.world->size = glm::vec2( 32 );
// init render data
ent.render->color = glm::vec3( 1.0f );
ent.render->textureName = "portal";
// init spawn data
ent.spawn->numRounds = 20;

return ent;
}
*/