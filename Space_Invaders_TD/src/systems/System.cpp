#include "systems/System.h"
#include "EntityFactory.h"
#include <algorithm>

System::System( unsigned* enemiesLeft ) {
	this->enemiesLeft = enemiesLeft;
}

System::~System() {
}

bool System::condition( unsigned componentTypes ) {
	return !( ( flags ^ componentTypes ) & flags );
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