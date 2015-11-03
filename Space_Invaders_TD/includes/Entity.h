#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

class Entity {
public:
	// list of possible componenets
	unsigned					componentTypes;
	HealthComponent				*health;
	WorldComponent				*world;
	RenderComponent				*render;
	MovementComponent			*movement;
	PathAIComponent				*path;
	SpawnComponent				*spawn;
	KeyboardInputComponent		*keyboard;

	Entity( unsigned components );
	~Entity();
private:
};

#endif // ENTITY_H
