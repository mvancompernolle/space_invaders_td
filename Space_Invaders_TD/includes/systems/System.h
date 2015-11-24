#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "World.h"
#include "Component.h"
#include "Entity.h"
#include <vector>
#include <string>
#include <mutex>

class System {
public:
	System( unsigned* enemiesLeft );
	~System();
	bool condition( unsigned componentTypes );
	virtual void update( World* world, int pos, float dt ) = 0;
	virtual void clear();
	void adjustEntityVector( World* world );
protected:
	std::string systemName;
	unsigned* enemiesLeft;
	unsigned int flags;
	std::mutex removalMutex, additionsMutex;

	void addRemoval( unsigned ent );
	void addAddition( const Entity& ent );
private:
	std::vector<int> removals;
	std::vector<Entity> additions;
};

#endif // SYSTEM_H
