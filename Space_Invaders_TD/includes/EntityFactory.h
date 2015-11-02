#pragma once

#ifndef ENTITY_FACTORY_H
#define	ENTITY_FACTORY_H

#include "Entity.h"
#include "World.h"
#include <vector>

class EntityFactory {

public:
	EntityFactory();
	~EntityFactory();
	static int createEntity( World* world, unsigned mask );
	static Entity createEnemy( World* world );
	static Entity createBaseTower();
	static Entity createPlayer();
	static Entity createSpawner();
private:
};

#endif // ENTITY_FACTORY