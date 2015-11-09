#pragma once

#ifndef ENTITY_FACTORY_H
#define	ENTITY_FACTORY_H

#include "Entity.h"
#include "World.h"
#include "CollisionSystem.h"
#include <vector>

class EntityFactory {

public:
	EntityFactory();
	~EntityFactory();
	static void setWorld( World* world );
	static void setCollisionSystem( CollisionSystem* collisionSystem );
	static void removeEntity( int pos );
	static int createEntity( unsigned mask );
	static int createEnemy();
	static int createBaseTower();
	static int createPlayer();
	static int createSpawner();
private:
	static World* world;
	static CollisionSystem* collisionSystem;
};

#endif // ENTITY_FACTORY