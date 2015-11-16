#pragma once

#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "System.h"

class SpawnSystem : public System {
public:
	SpawnSystem();
	~SpawnSystem();
	void update( World* world, int pos, float dt );
};

#endif // SPAWN_SYSTEM_H