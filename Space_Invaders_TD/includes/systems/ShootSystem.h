#pragma once

#ifndef SHOOT_SYSTEM_H
#define SHOOT_SYSTEM_H

#include "System.h"

class ShootSystem : public System {
public:
	ShootSystem( unsigned* enemiesLeft );
	~ShootSystem();
	void registerEntity( unsigned entity );
	void unregisterEntity( unsigned entity );
	void update( World* world, int pos, float dt );
private:
	void spawnBullets( World* world, unsigned pos, float dt );

	std::vector<unsigned> registeredEntities;
};

#endif // SHOOT_SYSTEM_H