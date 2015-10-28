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
	static std::vector<Entity>& getData();
	static Entity* createEnemy();
	static Entity* createBaseTower();
	static Entity* createPlayer();
private:
	static std::vector<Entity> entities; 
};

#endif // ENTITY_FACTORY