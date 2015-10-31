#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

class Entity {
public:
	// list of possible componenets
	static unsigned ID;
	unsigned myID;
	HealthComponent				*health;
	WorldComponent				*world;
	RenderComponent				*render;
	MovementComponent			*movement;
	PathAIComponent				*path;
	SpawnComponent				*spawn;
	KeyboardInputComponent		*keyboard;
	unsigned					componentTypes;

	Entity();
	Entity( const Entity& entity );
	Entity& operator=( const Entity& entity );
	bool	operator==( const Entity& entity );
	Entity( unsigned components );
	~Entity();
private:
};

#endif // ENTITY_H
