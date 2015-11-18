#include "systems/HealthSystem.h"



HealthSystem::HealthSystem( unsigned* enemiesLeft ) : System( enemiesLeft ) {
	flags = ( HEALTH );
}


HealthSystem::~HealthSystem() {
}

void HealthSystem::update( World* world, int pos, float dt ) {
	HealthComponent& healthComp = world->healthComponents[world->getComponentIndex( pos, HEALTH )];
	if ( healthComp.currHP <= 0 ) {
		removalMutex.lock();
		removals.push_back( pos );
		removalMutex.unlock();
	}
}