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
		EntityInfo() : mask( 0 ) {}
	};

	ComponentPool<EntityInfo, NUM_ENTITIES> entities;
	ComponentPool<WorldComponent, NUM_ENTITIES> worldComponents;
	ComponentPool<HealthComponent, NUM_ENTITIES / 4> healthComponents;
	ComponentPool<MovementComponent, 2048> movementComponents;
	ComponentPool<PathAIComponent, NUM_ENTITIES / 4> pathComponents;
	ComponentPool<SpawnComponent, 32> spawnComponents;
	ComponentPool<RenderComponent, NUM_ENTITIES> renderComponents;
	ComponentPool<PlayerInputComponent, 1> playerInputComponents;
	ComponentPool<CollisionComponent, NUM_ENTITIES / 2> collisionComponents;
	ComponentPool<DamageComponent, NUM_ENTITIES / 4> dmgComponents;
	ComponentPool<MoneyComponent, NUM_ENTITIES / 4> moneyComponents;

	int getComponentIndex( int pos, COMPONENT_TYPE type ) const {
		return entities[pos].componentIndices[type];
	}
};

#endif // WORLD_H