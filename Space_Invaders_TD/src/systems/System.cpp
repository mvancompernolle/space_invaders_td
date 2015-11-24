#include "systems/System.h"
#include "EntityFactory.h"
#include <algorithm>

System::System( unsigned* enemiesLeft ) : systemName("not named") {
	this->enemiesLeft = enemiesLeft;
}

System::~System() {
}

bool System::condition( unsigned componentTypes ) {
	return !( ( flags ^ componentTypes ) & flags );
}

void System::clear() {
	removals.clear();
	additions.clear();
}

void System::addRemoval( unsigned ent ) {
	removalMutex.lock();
	removals.push_back( ent );
	removalMutex.unlock();
}

void System::addAddition( const Entity& ent ) {
	additionsMutex.lock();
	additions.push_back( ent );
	additionsMutex.unlock();
}

void System::adjustEntityVector( World* world ) {
	// remove anything the system was supposed to remove
	for ( int i = 0; i < removals.size(); ++i ) {
		// delete an entity if not already deleted
		if ( world->entities[removals[i]].mask != NONE ) {
			if ( ( world->entities[removals[i]].mask | COLLISION ) 
				&& world->collisionComponents[world->getComponentIndex( removals[i], COLLISION )].collisionID == ENEMY ) {
				(*enemiesLeft)--;
			}
			EntityFactory::removeEntity( removals[i] );
		}
	}
	// add anything the system was supposed to add
	for ( int i = 0; i < additions.size(); i++ ) {
		EntityFactory::addEntity( additions[i] );
	}

	additions.clear();
	removals.clear();
}