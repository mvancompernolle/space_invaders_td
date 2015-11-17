#pragma once

#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "System.h"

class MovementSystem : public System {
public:
	MovementSystem( unsigned* enemiesLeft );
	~MovementSystem();
	void update( World* world, int pos, float dt );
};

#endif // MOVEMENT_SYSTEM

