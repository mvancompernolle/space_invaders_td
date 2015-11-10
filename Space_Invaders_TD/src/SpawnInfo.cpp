#include "SpawnInfo.h"
#include "Entity.h"

SpawnInfo::SpawnInfo() : spawnType( nullptr ) {}

SpawnInfo::~SpawnInfo() { if ( spawnType != nullptr ) delete spawnType; }

void SpawnInfo::setEntity( const Entity* ent ) {
	if ( spawnType != nullptr ) {
		delete spawnType;
	}
	spawnType = new Entity( *ent );
}

const Entity* SpawnInfo::getEntity() const {
	return spawnType;
}