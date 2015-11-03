#include "SpawnSystem.h"
#include "EntityFactory.h"



SpawnSystem::SpawnSystem() {
	flags = ( SPAWN );
}


SpawnSystem::~SpawnSystem() {
}

void SpawnSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	SpawnComponent& spawnComp = world->spawnComponents[world->getComponentIndex( pos, SPAWN )];

	if ( spawnComp.round < spawnComp.numRounds && spawnComp.currSpawnNum < spawnComp.spawnTypes[spawnComp.round].num ) {
		spawnComp.dt += dt;
		if ( spawnComp.dt >= spawnComp.spawnRate ) {
			spawnComp.dt -= spawnComp.spawnRate;

			// spawn an enemy
			Entity ent( HEALTH | RENDER | WORLD | MOVEMENT | PATH );

			// init world data
			ent.world->pos = worldComp.pos;
			ent.world->size = worldComp.size;
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
			additions.push_back( ent );

			// increment number of entities spawned
			spawnComp.currSpawnNum++;
		}
	}
}