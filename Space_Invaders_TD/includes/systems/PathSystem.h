#pragma once

#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "System.h"
#include "Grid.h"
#include <vector>

struct GridNode {
	glm::ivec2 prev, pos;
	float score;
	bool traversed, queued;

	friend bool operator<( const GridNode& l, const GridNode& r ) {
		return l.score > r.score;
	}
};

class PathSystem : public System {
public:
	PathSystem( unsigned* enemiesLeft );
	~PathSystem();
	void clear();
	void update( World* world, int pos, float dt );
	bool calcOptimalPath( glm::uvec2 start, glm::uvec2 end, float radius, const Grid& grid );
private:
	std::vector<glm::vec2> path;
	bool isCollisionBetween( glm::uvec2 curr, glm::uvec2 prev, float radius, const Grid& grid );
};

#endif AI_SYSTEM_H