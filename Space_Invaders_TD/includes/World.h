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

	ComponentPool<EntityInfo, 2048> entities;
	ComponentPool<WorldComponent, 2048> worldComponents;
	ComponentPool<HealthComponent, 256> healthComponents;
	ComponentPool<MovementComponent, 2048> movementComponents;
	ComponentPool<PathAIComponent, 256> pathComponents;
	ComponentPool<SpawnComponent, 32> spawnComponents;
};

#endif // WORLD_H