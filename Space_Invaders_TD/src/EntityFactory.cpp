#include "EntityFactory.h"

std::vector<Entity> EntityFactory::entities = std::vector<Entity>();

EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

std::vector<Entity>& EntityFactory::getData() {
	return entities;
}

Entity EntityFactory::createEnemy() {
	Entity ent( HEALTH | RENDER | WORLD | MOVEMENT | PATH );
	
	// init world data
	ent.world->pos = glm::vec2( 0 );
	ent.world->size = glm::vec2( 32 ); 
	// init health data
	ent.health->currHP = 100.0f;
	ent.health->maxHP = 100.0f;
	// init movement data
	ent.movement->vel = glm::vec2( 350.0f, 0.0f );
	ent.movement->defSpeed = 300.0f;
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
	Entity ent( RENDER | WORLD | MOVEMENT );
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