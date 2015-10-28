#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

class Entity {
public:
	// list of possible componenets
	HealthComponent				*health;
	WorldComponent				*world;
	RenderComponent				*render;
	MovementComponent			*movement;
	AIComponent					*ai;
	unsigned					componentTypes;

	Entity();
	Entity( const Entity& entity );
	Entity& operator=( const Entity& entity );
	Entity( unsigned components );
	~Entity();
private:
};

#endif // ENTITY_H
