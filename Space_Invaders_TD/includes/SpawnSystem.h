#pragma once

#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "System.h"

class SpawnSystem : public System {
public:
	SpawnSystem();
	~SpawnSystem();
	void update( const Entity& entity, float dt );
};

#endif // SPAWN_SYSTEM_H