#pragma once

#ifndef SHOOT_SYSTEM_H
#define SHOOT_SYSTEM_H

#include "System.h"

class ShootSystem : public System {
public:
	ShootSystem();
	~ShootSystem();
	void registerEntity( unsigned entity );
	void unregisterEntity( unsigned entity );
	void update( World* world, int pos, float dt );
private:
	std::vector<unsigned> registeredEntities;
};

#endif // SHOOT_SYSTEM_H