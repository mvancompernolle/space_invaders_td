#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"
#include "Component.h"
#include <vector>

class System {
public:
	System();
	~System();
	bool condition( unsigned componentTypes );
	virtual void update( const Entity& entity, float dt ) = 0;
protected:
	unsigned int flags;
};

#endif // SYSTEM_H
