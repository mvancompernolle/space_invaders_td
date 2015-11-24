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
	ComponentPool<SlowedComponent, NUM_ENTITIES / 4> slowComponents;
	ComponentPool<AOEComponent, NUM_ENTITIES / 4> AOEComponents;
	ComponentPool<ParentComponent, NUM_ENTITIES / 4> parentComponents;
	ComponentPool<RotationComponent, NUM_ENTITIES / 4> rotationComponents;
	std::unordered_map<COMPONENT_TYPE, unsigned> componentIndexMap;

	unsigned getComponentIndex( int pos, COMPONENT_TYPE type ) const {
		return entities[pos].componentIndices[componentIndexMap.at(type)];
	}

	void setComponentIndex( int pos, COMPONENT_TYPE type, unsigned index ) {
		entities[pos].componentIndices[componentIndexMap[type]] = index;
	}

	void printComponentIndices( int pos ) {
		for ( int i = 0; i < componentIndexMap.size(); ++i ) {
			std::cout << "index: " << entities[pos].componentIndices[i] << std::endl;
		}
		std::cout << std::endl;
	}

	World() : entities("entities"), worldComponents("world"), healthComponents("health"), movementComponents("movement"),
		pathComponents("path"), spawnComponents("spawn"), renderComponents("render"), playerInputComponents("player"), 
		collisionComponents("collision"), dmgComponents("dmg"), moneyComponents("money"), shootComponents("shoot"), 
		followComponents("follow"), dmgAuraComponents("dmg_aura"), slowComponents("slowed"), AOEComponents("aoe"), 
		parentComponents("parent"), rotationComponents("rotation"){
		componentIndexMap[HEALTH] = 0;
		componentIndexMap[WORLD] = 1;
		componentIndexMap[RENDER] = 2;
		componentIndexMap[MOVEMENT] = 3;
		componentIndexMap[PATH] = 4;
		componentIndexMap[SPAWN] = 5;
		componentIndexMap[COLLISION] = 6;
		componentIndexMap[PLAYER_INPUT] = 7;
		componentIndexMap[DAMAGE] = 8;
		componentIndexMap[MONEY] = 9;
		componentIndexMap[SHOOT] = 10;
		componentIndexMap[FOLLOW] = 11;
		componentIndexMap[DAMAGE_AURA] = 12;
		componentIndexMap[SLOWED] = 13;
		componentIndexMap[AOE] = 14;
		componentIndexMap[PARENT] = 15;
		componentIndexMap[ROTATION] = 16;
	}

	void clear() {
		entities.clear();
		worldComponents.clear();
		healthComponents.clear();
		movementComponents.clear();
		pathComponents.clear();
		spawnComponents.clear();
		renderComponents.clear();
		playerInputComponents.clear();
		collisionComponents.clear();
		dmgComponents.clear();
		moneyComponents.clear();
		shootComponents.clear();
		followComponents.clear();
		dmgAuraComponents.clear();
		slowComponents.clear();
		AOEComponents.clear();
		parentComponents.clear();
		rotationComponents.clear();
	}
};

#endif // WORLD_H