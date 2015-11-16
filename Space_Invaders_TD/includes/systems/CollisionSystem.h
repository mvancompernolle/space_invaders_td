#pragma once

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "System.h"

#include <glm/glm.hpp>
#include <vector>

enum EVENT_TYPE {
	DAMAGE_EVENT,
	DESPAWN_EVENT
};

struct CollisionEvent {
	unsigned ent1, ent2;
	EVENT_TYPE eventType;
	CollisionEvent( unsigned ent1, unsigned ent2, EVENT_TYPE type ) : ent1( ent1 ), ent2( ent2 ), eventType( type ) { }
};

class CollisionSystem {
public:
	std::vector<CollisionEvent> collisions;
	CollisionSystem();
	~CollisionSystem();
	void clearCollisionEvents();
	void registerEntity( unsigned entity );
	void unregisterEntity( unsigned entity );
	void update( World* world );
	static bool arePolygonsIntersecting( std::vector<glm::vec2> p1, std::vector<glm::vec2> p2 );
	static bool areRectsIntersecting( glm::vec2 pos1, glm::vec2 pos2, glm::vec2 size1, glm::vec2 size2 );
	static bool areCirclesIntersecting( glm::vec2 pos1, glm::vec2 pos2, float radius1, float radius2 );
	static bool areRectCircleIntersecting( glm::vec2 rectPos, glm::vec2 cirPos, glm::vec2 rectSize, float cirRadius, float rectRot );
private: 
	unsigned flags;
	std::vector<unsigned> registeredEntities;

	glm::vec2 getCenter( glm::vec2 pos, glm::vec2 size ) const;
	void createCollisionEvents( const World& world, unsigned ent1, unsigned ent2 );
};

#endif //COLLISION_SYSTEM_H