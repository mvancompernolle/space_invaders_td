#pragma once

#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "System.h"

class MovementSystem : public System {
public:
	MovementSystem();
	~MovementSystem();
	void update( const Entity& entity, float dt );
};

#endif // MOVEMENT_SYSTEM

