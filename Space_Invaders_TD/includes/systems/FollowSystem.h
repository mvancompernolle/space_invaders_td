#pragma once

#ifndef FOLLOW_SYSTEM_H
#define FOLLOW_SYSTEM_H

#include "System.h"
class FollowSystem : public System {
public:
	FollowSystem( unsigned* enemiesLeft );
	~FollowSystem();
	void update( World* world, int pos, float dt );
};

#endif // FOLLOW_SYSTEM_H