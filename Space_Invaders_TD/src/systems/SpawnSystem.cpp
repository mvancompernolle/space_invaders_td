#include "systems/SpawnSystem.h"
#include "EntityFactory.h"



SpawnSystem::SpawnSystem( unsigned* enemiesLeft ) : System( enemiesLeft ){
	flags = ( SPAWN );
	systemName = "spawnSystem";
}


SpawnSystem::~SpawnSystem() {
}

void SpawnSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	SpawnComponent& spawnComp = world->spawnComponents[world->getComponentIndex( pos, SPAWN )];

	if ( spawnComp.round < spawnComp.numRounds && spawnComp.currSpawnNum < spawnComp.spawnTypes[spawnComp.round].num ) {
		spawnComp.dt += dt;
		if ( spawnComp.dt >= spawnComp.spawnTypes[spawnComp.round].spawnRate ) {
			spawnComp.dt -= spawnComp.spawnTypes[spawnComp.round].spawnRate;

			// increment number of entities spawned
			spawnComp.currSpawnNum++;
			addAddition( *spawnComp.spawnTypes[spawnComp.round].getEntity() );
		}
	}
}