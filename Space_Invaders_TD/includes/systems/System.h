#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "World.h"
#include "Component.h"
#include "Entity.h"
#include <vector>
#include <mutex>

class System {
public:
	System( unsigned* enemiesLeft );
	~System();
	bool condition( unsigned componentTypes );
	virtual void update( World* world, int pos, float dt ) = 0;
	void adjustEntityVector( World* world );
protected:
	unsigned* enemiesLeft;
	unsigned int flags;
	std::vector<int> removals;
	std::vector<Entity> additions;
	std::mutex removalMutex, additionsMutex;
};

#endif // SYSTEM_H
