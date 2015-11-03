#include "System.h"
#include "EntityFactory.h"
#include <algorithm>

System::System( ) {
}

System::~System() {
}

bool System::condition( unsigned componentTypes ){
	return !( ( flags ^ componentTypes ) & flags );
}

void System::adjustEntityVector( World* world ) {
	// remove anything the system was supposed to remove
	for ( int i = 0; i < removals.size(); ++i ) {
		// delete an entity if not already deleted
		if ( world->entities[removals[i]].mask != NONE ) {
			world->removeEntity( removals[i] );
		}
	}
	// add anything the system was supposed to add
	for ( int i = 0; i < additions.size(); i++ ) {
		int entPos = EntityFactory::createEntity( world, additions[i].componentTypes );
		int pos = 0;
		for ( int j = 1; j < COMPONENT_SIZE; j <<= 1 ) {
			switch ( j & additions[i].componentTypes ) {
			case HEALTH:
				world->healthComponents[ world->getComponentIndex( entPos, HEALTH ) ] = additions[i].health;
				break;
			case WORLD:
				world->worldComponents[world->getComponentIndex( entPos, WORLD )] = additions[i].world;
				break;
			case RENDER:
				world->renderComponents[world->getComponentIndex( entPos, RENDER )] = additions[i].render;
				break;
			case MOVEMENT:
				world->movementComponents[world->getComponentIndex( entPos, MOVEMENT )] = additions[i].movement;
				break;
			case PATH:
				world->pathComponents[world->getComponentIndex( entPos, PATH )] = additions[i].path;
				break;
			case SPAWN:
				world->spawnComponents[world->getComponentIndex( entPos, SPAWN )] = additions[i].spawn;
				break;
			}
		}
	}

	additions.clear();
	removals.clear();
}