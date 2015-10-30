#pragma once

#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "System.h"
#include "Grid.h"
#include <vector>

enum DIRECTION {
	LEFT, RIGHT, UP, DOWN
};

struct GridNode {
	glm::ivec2 prev, pos;
	unsigned score;
	bool traversed;

	friend bool operator<( const GridNode& l, const GridNode& r ) {
		return l.score > r.score;
	}
};

class PathSystem : public System {
public:
	PathSystem();
	~PathSystem();
	void update( const Entity& entity, float dt );
	bool calcOptimalPath( glm::uvec2 start, glm::uvec2 end, const Grid grid );
private:
	std::vector<glm::vec2> path;

	DIRECTION getDirection( glm::uvec2 pos1, glm::uvec2 pos2 );
	bool isDirection( glm::uvec2 pos1, glm::uvec2 pos2, DIRECTION dir );
};

#endif AI_SYSTEM_H