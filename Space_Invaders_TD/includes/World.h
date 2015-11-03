#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "ComponentPool.h"
#include <vector>

#define NUM_ENTITIES 2048

struct World {
	struct EntityInfo {
		unsigned mask;
		unsigned componentIndices[COMPONENT_SIZE];
	};

	ComponentPool<EntityInfo, NUM_ENTITIES> entities;
	ComponentPool<WorldComponent, 2048> worldComponents;
	ComponentPool<HealthComponent, 256> healthComponents;
	ComponentPool<MovementComponent, 2048> movementComponents;
	ComponentPool<PathAIComponent, 256> pathComponents;
	ComponentPool<SpawnComponent, 32> spawnComponents;
	ComponentPool<RenderComponent, 2048> renderComponents;

	int getComponentIndex( int pos, COMPONENT_TYPE type ) {
		return entities[pos].componentIndices[type];
	}

	void removeEntity( int pos ) {
		for ( int i = 1; i < COMPONENT_SIZE; i <<= 1 ) {
			switch ( i & entities[pos].mask ) {
			case HEALTH:
				healthComponents.remove( getComponentIndex( pos, HEALTH) );
				break;
			case WORLD:
				worldComponents.remove( getComponentIndex( pos, WORLD ) );
				break;
			case RENDER:
				renderComponents.remove( getComponentIndex( pos, RENDER ) );
				break;
			case MOVEMENT:
				movementComponents.remove( getComponentIndex( pos, MOVEMENT ) );
				break;
			case PATH:
				pathComponents.remove( getComponentIndex( pos, PATH ) );
				break;
			case SPAWN:
				spawnComponents.remove( getComponentIndex( pos, SPAWN ) );
			}
		}
		entities[pos].mask = NONE;
		entities.remove( pos );
	}
};

#endif // WORLD_H