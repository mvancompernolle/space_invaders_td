#pragma once

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "System.h"

#include <glm/glm.hpp>
#include <vector>

class CollisionSystem : public System {
public:
	CollisionSystem();
	~CollisionSystem();
	void update( World* world, int pos, float dt );
	static bool arePolygonsIntersecting( std::vector<glm::vec2> p1, std::vector<glm::vec2> p2 );
};

#endif //COLLISION_SYSTEM_H