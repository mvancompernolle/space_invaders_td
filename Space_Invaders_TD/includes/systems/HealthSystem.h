#pragma once

#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include "System.h"
class HealthSystem : public System {
public:
	HealthSystem();
	~HealthSystem();
	void update( World* world, int pos, float dt );
};

#endif // HEALTH_SYSTEM_H
