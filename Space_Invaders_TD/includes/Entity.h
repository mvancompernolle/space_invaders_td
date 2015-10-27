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
	unsigned					componentTypes;

	Entity();
	Entity( unsigned components );
	~Entity();
private:
};

#endif // ENTITY_H
