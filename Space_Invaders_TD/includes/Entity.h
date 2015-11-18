#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

class Entity {
public:
	// list of possible componenets
	unsigned					componentTypes;
	HealthComponent				health;
	WorldComponent				world;
	RenderComponent				render;
	MovementComponent			movement;
	PathAIComponent				path;
	SpawnComponent				spawn;
	PlayerInputComponent        playerInput;
	CollisionComponent			collision;
	DamageComponent				damage;
	MoneyComponent				money;
	ShootComponent				shoot;
	FollowComponent				follow;
	DmgAuraComponent			dmgAura;
	SlowedComponent				slowed;
	AOEComponent				aoe;

	Entity( unsigned components );
	~Entity();
private:
};

#endif // ENTITY_H
