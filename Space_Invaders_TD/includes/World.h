#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "ComponentPool.h"
#include <vector>
#include <unordered_map>

#define NUM_ENTITIES 2048

struct World {
	struct EntityInfo {
		unsigned mask;
		EntityInfo() : mask( 0 ) {}
		friend class World;
	private:
		unsigned componentIndices[COMPONENT_NUM];
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
	ComponentPool<ShootComponent, NUM_ENTITIES / 4> shootComponents;
	ComponentPool<FollowComponent, NUM_ENTITIES / 4> followComponents;
	ComponentPool<DmgAuraComponent, NUM_ENTITIES / 4> dmgAuraComponents;
	std::unordered_map<COMPONENT_TYPE, unsigned> componentIndexMap;

	int getComponentIndex( int pos, COMPONENT_TYPE type ) const {
		return entities[pos].componentIndices[componentIndexMap.at(type)];
	}

	void setComponentIndex( int pos, COMPONENT_TYPE type, unsigned index ) {
		entities[pos].componentIndices[componentIndexMap[type]] = index;
	}

	World() : entities("entities"), worldComponents("world"), healthComponents("health"), movementComponents("movement"),
		pathComponents("path"), spawnComponents("spawn"), renderComponents("render"), playerInputComponents("player"), 
		collisionComponents("collision"), dmgComponents("dmg"), moneyComponents("money"), shootComponents("shoot"), 
		followComponents("follow"), dmgAuraComponents("dmg_aura"){
		componentIndexMap[HEALTH] = 0;
		componentIndexMap[WORLD] = 1;
		componentIndexMap[RENDER] = 2;
		componentIndexMap[PATH] = 3;
		componentIndexMap[SPAWN] = 4;
		componentIndexMap[COLLISION] = 5;
		componentIndexMap[PLAYER_INPUT] = 6;
		componentIndexMap[DAMAGE] = 7;
		componentIndexMap[MONEY] = 8;
		componentIndexMap[SHOOT] = 9;
		componentIndexMap[FOLLOW] = 10;
		componentIndexMap[DAMAGE_AURA] = 11;
	}
};

#endif // WORLD_H