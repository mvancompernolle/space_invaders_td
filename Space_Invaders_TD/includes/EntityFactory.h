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
	static int createEnemy( World* world );
	static int createBaseTower( World* world );
	static int createPlayer( World* world );
	static int createSpawner( World* world );
private:
};

#endif // ENTITY_FACTORY