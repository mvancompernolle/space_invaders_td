#include "SpawnSystem.h"
#include "EntityFactory.h"


SpawnSystem::SpawnSystem() {
	flags = ( SPAWN );
}


SpawnSystem::~SpawnSystem() {
}

void SpawnSystem::update( const Entity& entity, float dt ){
	if ( entity.spawn->round < entity.spawn->numRounds && entity.spawn->currSpawnNum < entity.spawn->spawnTypes[entity.spawn->round].num ) {
		entity.spawn->dt += dt;
		if ( entity.spawn->dt >= entity.spawn->spawnRate ) {
			entity.spawn->dt -= entity.spawn->spawnRate;

			// spawn an enemy
			Entity ent = EntityFactory::createEnemy();
			ent.world->pos = glm::vec2( entity.world->pos );
			ent.world->size = glm::vec2( entity.world->size );
			additions.push_back( ent );
			entity.spawn->currSpawnNum++;
		}
	}
}