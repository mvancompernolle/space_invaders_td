#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"

class System {
public:
	System( EntityManager* mgr );
	~System();
	virtual void update( float dt ) = 0;
private:
	EntityManager* entities;
};

#endif // SYSTEM_H
