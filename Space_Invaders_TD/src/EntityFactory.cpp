#include "EntityFactory.h"

std::vector<Entity> EntityFactory::entities = std::vector<Entity>();

EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

std::vector<Entity>& EntityFactory::getData() {
	return entities;
}

Entity* EntityFactory::createEnemy() {
	Entity ent( HEALTH | RENDER | WORLD | MOVEMENT | AI );
	
	// init world data
	ent.world->pos = glm::vec2( 0 );
	ent.world->size = glm::vec2( 32 ); 
	ent.world->shape = RECTANGLE;
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
	ent.ai->target = glm::vec2( 0.0f, 0.0f );
	ent.ai->AIType = ENEMY;

	entities.push_back( ent );

	return &ent;
}

Entity* EntityFactory::createBaseTower() {
	Entity ent( RENDER | WORLD | AI );

	// init world data
	ent.world->size = glm::vec2( 64 );
	ent.world->shape = RECTANGLE;
	ent.world->rotation = 0.0f;
	// init render data
	ent.render->color = glm::vec3( 1.0f );
	ent.render->textureName = "tower_base";
	// init ai data
	ent.ai->target = glm::vec2( 500.0f );
	ent.ai->AIType = TOWER;

	entities.push_back( ent );
	return &entities.back();
}

Entity* EntityFactory::createPlayer() {
	Entity ent( RENDER | WORLD | MOVEMENT );
	return &ent;
}