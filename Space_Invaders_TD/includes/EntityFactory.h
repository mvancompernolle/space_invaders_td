#pragma once

#ifndef ENTITY_FACTORY_H
#define	ENTITY_FACTORY_H

#include "Entity.h"
#include "World.h"
#include "CollisionSystem.h"
#include "ShootSystem.h"
#include <vector>

class EntityFactory {

public:
	EntityFactory();
	~EntityFactory();
	static void setWorld( World* world );
	static void setCollisionSystem( CollisionSystem* collisionSystem );
	static void setShootSystem( ShootSystem* shootSystem );
	static void removeEntity( int pos );
	static int createEntity( unsigned mask );
	static int createEnemy();
	static int createBaseTower();
	static int createPlayer();
	static int createSpawner();
	static void addEntity( Entity ent );
private:
	static World* world;
	static CollisionSystem* collisionSystem;
	static ShootSystem* shootSystem;
};

#endif // ENTITY_FACTORY