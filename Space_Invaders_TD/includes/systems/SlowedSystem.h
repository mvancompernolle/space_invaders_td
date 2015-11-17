#pragma once

#ifndef SLOW_SYSTEM_H
#define SLOW_SYSTEM_H

#include "systems/System.h"

class SlowedSystem : public System {
public:
	SlowedSystem( unsigned* enemiesLeft );
	~SlowedSystem();
	void update( World* world, int pos, float dt );
};

#endif // SLOW_SYSTEM_H

