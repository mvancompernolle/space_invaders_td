#include "PathSystem.h"
#include <cassert>
#include <queue>
#include <algorithm>

PathSystem::PathSystem() {
	flags = PATH | WORLD;
}


PathSystem::~PathSystem() {
}

void PathSystem::update( const Entity& entity, float dt ) {
	if ( glm::distance( entity.world->pos, entity.path->target ) < entity.world->size.x * 0.2f ) {
		//entity.path->pathIndex = (entity.path->pathIndex + 1) % path.size();
		entity.path->pathIndex++;
		if ( entity.path->pathIndex == path.size() ) {
			removals.push_back( entity );
		} else {
			entity.path->target = path[entity.path->pathIndex];
		}
	} else {
		// make sure to head towards target
		assert( entity.world != nullptr );
		entity.movement->vel = glm::normalize( entity.path->target - entity.world->pos ) * glm::length(entity.movement->vel);
	}
}

bool PathSystem::calcOptimalPath( glm::uvec2 start, glm::uvec2 end, const Grid grid ) {
	GridNode** gridNodes = new GridNode*[grid.size()];
	unsigned** heuristic = new unsigned*[grid.size()];
	for ( int i = 0; i < grid.size(); ++i ) {
		gridNodes[i] = new GridNode[grid[i].size()];
		heuristic[i] = new unsigned[grid[i].size()];

		// init values
		for ( int j = 0; j < grid[i].size(); ++j ) {
			gridNodes[i][j].score = ~0;
			gridNodes[i][j].prev = glm::vec2(-1);
			gridNodes[i][j].traversed = false;
			gridNodes[i][j].pos = glm::uvec2( j, i );
		}
	}

	// calculate heuristic for each spot on grid
	for ( int i = 0; i < grid.size(); ++i ) {
		for ( int j = 0; j < grid.size(); ++j ) {
			heuristic[i][j] = std::abs( (int)(start.x - end.x) ) + std::abs( (int)(start.y - end.y) );
		}
	}
	path.clear();

	// create a priority queue for the open list
	std::priority_queue<GridNode> open;

	// add start point to the open list
	open.push( gridNodes[start.y][start.x] );

	while ( !open.empty() ) {
		// get the current node off the end of the queue and remove it
		GridNode node = open.top();
		open.pop();
		// mark it as traversed
		gridNodes[node.pos.y][node.pos.x].traversed = true;
		// down
		if ( node.pos.y < grid.size() - 1 && !grid[node.pos.y + 1][node.pos.x].taken && !gridNodes[node.pos.y+1][node.pos.x].traversed ) {
			// calculate the nodes cost based on previous nodes cost
			if ( node.score + 1 < gridNodes[node.pos.y + 1][node.pos.x].score ) {
				// set the nodes cost
				gridNodes[node.pos.y + 1][node.pos.x].score = node.score + 1;
				// set the nodes previous to the current node
				gridNodes[node.pos.y + 1][node.pos.x].prev = node.pos;
				// put in the queue if not already
				open.push( gridNodes[node.pos.y + 1][node.pos.x] );
			}
		}
		// up
		if ( node.pos.y > 0 && !grid[node.pos.y - 1][node.pos.x].taken && !gridNodes[node.pos.y - 1][node.pos.x].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y - 1][node.pos.x].score ) {
				gridNodes[node.pos.y - 1][node.pos.x].score = node.score + 1;
				gridNodes[node.pos.y - 1][node.pos.x].prev = node.pos;
				open.push( gridNodes[node.pos.y - 1][node.pos.x] );
			}
		}
		// left
		if ( node.pos.x > 0 && !grid[node.pos.y][node.pos.x - 1].taken && !gridNodes[node.pos.y][node.pos.x - 1].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y][node.pos.x - 1].score ) {
				gridNodes[node.pos.y][node.pos.x-1].score = node.score + 1;
				gridNodes[node.pos.y][node.pos.x-1].prev = node.pos;
				open.push( gridNodes[node.pos.y][node.pos.x-1] );
			}
		}
		// right
		if ( node.pos.x < grid[0].size() - 1 && !grid[node.pos.y][node.pos.x + 1].taken && !gridNodes[node.pos.y][node.pos.x + 1].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y][node.pos.x + 1].score ) {
				gridNodes[node.pos.y][node.pos.x + 1].score = node.score + 1;
				gridNodes[node.pos.y][node.pos.x + 1].prev = node.pos;
				open.push( gridNodes[node.pos.y][node.pos.x + 1] );
			}
		}
	}

	// set the path calculate if there was one
	bool pathFound = true;
	glm::uvec2 currPos = end;
	while ( gridNodes[currPos.y][currPos.x].pos != (glm::ivec2) start ) {
		if ( gridNodes[currPos.y][currPos.x].prev == glm::ivec2( -1 ) ) {
			pathFound = false;
			break;
		}
		path.push_back( glm::vec2(grid[currPos.y][currPos.x].pos.y, grid[currPos.y][currPos.x].pos.x) );
		currPos = gridNodes[currPos.y][currPos.x].prev;
	}
	std::reverse( path.begin(), path.end() );

	for ( int i = 0; i < grid.size(); ++i ) {
		delete[] gridNodes[i];
		delete[] heuristic[i];
	}
	delete[] gridNodes;
	delete[] heuristic;

	return pathFound;
}
