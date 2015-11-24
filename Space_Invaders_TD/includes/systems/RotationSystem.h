#pragma once

#ifndef ROTATION_SYSTEM_H
#define ROTATION_SYSTEM_H

#include "System.h"

class RotationSystem : public System {
public:
	RotationSystem( unsigned* enemiesLeft );
	~RotationSystem();
	void update( World* world, int pos, float dt );
};

#endif // ROTATION_SYSTEM
