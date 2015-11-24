#pragma once

#ifndef ENTITY_FACTORY_H
#define	ENTITY_FACTORY_H

#include "Entity.h"
#include "World.h"
#include "systems/CollisionSystem.h"
#include "systems/ShootSystem.h"
#include "systems/DamageAuraSystem.h"
#include <vector>

class EntityFactory {

public:
	EntityFactory();
	~EntityFactory();
	static void setWorld( World* world );
	static void setCollisionSystem( CollisionSystem* collisionSystem );
	static void setShootSystem( ShootSystem* shootSystem );
	static void setDmgAuraSystem( DamageAuraSystem* dmgAuraSystem );

	static void removeEntity( int pos );
	static int createEntity( unsigned mask );
	static int createEnemy();
	static int createBaseTower();
	static int createPlayer();
	static int createSpawner();
	static unsigned addEntity( Entity ent );
	static unsigned addComponent( unsigned pos, COMPONENT_TYPE type );
	static void resetComponent( unsigned pos, COMPONENT_TYPE type );
private:
	static World* world;
	static CollisionSystem* collisionSystem;
	static ShootSystem* shootSystem;
	static DamageAuraSystem* dmgAuraSystem;
};

#endif // ENTITY_FACTORY