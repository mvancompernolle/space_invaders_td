#pragma once

#ifndef ENTITY_FACTORY_H
#define	ENTITY_FACTORY_H

#include "Entity.h"
#include "Component.h"
#include <vector>

class EntityFactory {

public:
	EntityFactory();
	~EntityFactory();
	static Entity createEnemy();
	static Entity createBaseTower();
	static Entity createPlayer();
	static Entity createSpawner();
};

#endif // ENTITY_FACTORY