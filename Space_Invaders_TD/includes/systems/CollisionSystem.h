#pragma once

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "System.h"

#include <glm/glm.hpp>
#include <vector>
#include <list>

enum EVENT_TYPE {
	DAMAGE_EVENT,
	DESPAWN_EVENT,
	AOE_SLOW_EVENT,
	AOE_DAMAGE_EVENT
};

struct CollisionEvent {
	unsigned ent1, ent2;
	unsigned type1, type2;
	EVENT_TYPE eventType;
	CollisionEvent( unsigned ent1, unsigned ent2, unsigned type1, unsigned type2, EVENT_TYPE type )
		: ent1( ent1 ), ent2( ent2 ), type1( type1 ), type2( type2 ), eventType( type ) {
	}
};

class CollisionSystem {
public:
	std::vector<CollisionEvent> collisions;
	std::vector<unsigned> registeredEntities;
	std::vector<unsigned> registeredEnemies;
	CollisionSystem();
	~CollisionSystem();
	void clearCollisionEvents();
	void registerEntity( unsigned entity );
	void unregisterEntity( unsigned entity );
	void registerEnemy( unsigned entity );
	void unregisterEnemy( unsigned entity );
	void update( World* world );
	void clear();
	static bool arePolygonsIntersecting( std::vector<glm::vec2> p1, std::vector<glm::vec2> p2 );
	static bool areRectsIntersecting( glm::vec2 pos1, glm::vec2 pos2, glm::vec2 size1, glm::vec2 size2 );
	static bool areCirclesIntersecting( glm::vec2 pos1, glm::vec2 pos2, float radius1, float radius2 );
	static bool areRectCircleIntersecting( glm::vec2 rectPos, glm::vec2 cirPos, glm::vec2 rectSize, float cirRadius, float rectRot );
private:
	unsigned flags;

	void calcCollisions( World* world, int start, int end, std::list<CollisionEvent>& results ) const;
	glm::vec2 getCenter( glm::vec2 pos, glm::vec2 size ) const;
	void createCollisionEvents( const World& world, unsigned ent1, unsigned ent2, std::list<CollisionEvent>& results ) const;
};

#endif //COLLISION_SYSTEM_H