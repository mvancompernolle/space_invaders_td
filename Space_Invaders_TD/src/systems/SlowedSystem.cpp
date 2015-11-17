#include "systems/SlowedSystem.h"
#include <algorithm>

SlowedSystem::SlowedSystem( unsigned* enemiesLeft ) : System(enemiesLeft) {
	flags = MOVEMENT | SLOWED;
}


SlowedSystem::~SlowedSystem() {
}

void SlowedSystem::update( World* world, int pos, float dt ) {
	// loop over slows 
	MovementComponent& moveComp = world->movementComponents[world->getComponentIndex( pos, MOVEMENT )];
	SlowedComponent& slowedComp = world->slowComponents[world->getComponentIndex( pos, SLOWED )];
	
	std::vector<unsigned> slowToRemove;
	int i = 0;
	for ( SlowInfo& info : slowedComp.slowedInfo ) {
		info.timeLeft -= dt;
		// remove the slow effect
		if ( info.timeLeft <= 0.0f ) {
			// restore speed to the object
			moveComp.vel += info.percentSpeed * moveComp.defSpeed;
			slowToRemove.push_back( i );
		}
		++i;
	}

	// remove timed out slows
	for ( unsigned pos : slowToRemove ) {
		slowedComp.slowedInfo.erase( slowedComp.slowedInfo.begin() + pos );
	}
}