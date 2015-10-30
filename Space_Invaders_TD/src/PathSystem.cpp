#include "PathSystem.h"
#include <cassert>
#include <queue>
#include <algorithm>
#include <iostream>

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
			entity.path->target = path[entity.path->pathIndex] - entity.world->size / 2.0f;
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
			heuristic[i][j] = std::abs( (int)(j - end.x) ) + std::abs( (int)(i - end.y) );
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
				gridNodes[node.pos.y + 1][node.pos.x].score = node.score + heuristic[node.pos.y + 1][node.pos.x];
				// set the nodes previous to the current node
				gridNodes[node.pos.y + 1][node.pos.x].prev = node.pos;
				// put in the queue if not already
				open.push( gridNodes[node.pos.y + 1][node.pos.x] );
			}
		}
		// up
		if ( node.pos.y > 0 && !grid[node.pos.y - 1][node.pos.x].taken && !gridNodes[node.pos.y - 1][node.pos.x].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y - 1][node.pos.x].score ) {
				gridNodes[node.pos.y - 1][node.pos.x].score = node.score + heuristic[node.pos.y - 1][node.pos.x];
				gridNodes[node.pos.y - 1][node.pos.x].prev = node.pos;
				open.push( gridNodes[node.pos.y - 1][node.pos.x] );
			}
		}
		// left
		if ( node.pos.x > 0 && !grid[node.pos.y][node.pos.x - 1].taken && !gridNodes[node.pos.y][node.pos.x - 1].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y][node.pos.x - 1].score ) {
				gridNodes[node.pos.y][node.pos.x-1].score = node.score + heuristic[node.pos.y][node.pos.x - 1];
				gridNodes[node.pos.y][node.pos.x-1].prev = node.pos;
				open.push( gridNodes[node.pos.y][node.pos.x-1] );
			}
		}
		// right
		if ( node.pos.x < grid[0].size() - 1 && !grid[node.pos.y][node.pos.x + 1].taken && !gridNodes[node.pos.y][node.pos.x + 1].traversed ) {
			if ( node.score + 1 < gridNodes[node.pos.y][node.pos.x + 1].score ) {
				gridNodes[node.pos.y][node.pos.x + 1].score = node.score + heuristic[node.pos.y][node.pos.x + 1];
				gridNodes[node.pos.y][node.pos.x + 1].prev = node.pos;
				open.push( gridNodes[node.pos.y][node.pos.x + 1] );
			}
		}
	}

	// perform first pass over path to smoothe it
	bool pathFound = true;
	glm::uvec2 currPos = end;
	std::vector<glm::uvec2> tiles;
	std::vector<std::pair<glm::uvec2, unsigned>> pathVals;
	while ( gridNodes[currPos.y][currPos.x].pos != (glm::ivec2) start ) {
		glm::ivec2& prev = gridNodes[currPos.y][currPos.x].prev;
		if ( prev == glm::ivec2( -1 ) ) {
			pathFound = false;
			break;
		}

		// peform path smoothing by not adding points that don't cause the path to go diagonally accross walls
		bool skipPathNode = true;
		// prev up left
		if ( currPos != start && currPos != end && prev != (glm::ivec2) start) {
			// get the directions of the prev and next nodes
			DIRECTION currToPrev = getDirection( tiles.back(), currPos );
			DIRECTION currToNext = getDirection( (glm::uvec2) gridNodes[currPos.y][currPos.x].prev, currPos );
			// check cases
			if ( ( ( ( isDirection( tiles.back(), currPos, UP ) && currToNext == RIGHT ) || ( isDirection( tiles.back(), currPos, RIGHT ) && currToNext == UP ) ) && grid[currPos.y - 1][currPos.x + 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, DOWN ) && currToNext == RIGHT ) || ( isDirection( tiles.back(), currPos, RIGHT ) && currToNext == DOWN ) ) && grid[currPos.y + 1][currPos.x + 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, UP ) && currToNext == LEFT ) || ( isDirection( tiles.back(), currPos, LEFT ) && currToNext == UP ) ) && grid[currPos.y - 1][currPos.x - 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, DOWN ) && currToNext == LEFT ) || ( isDirection( tiles.back(), currPos, LEFT ) && currToNext == DOWN ) ) && grid[currPos.y + 1][currPos.x - 1].taken ) ) {
				skipPathNode = false;
			}
		} else {
			skipPathNode = false;
		}

		if ( !skipPathNode ) {
			tiles.push_back( currPos );
			pathVals.push_back( std::make_pair(currPos, 0) );
		} else {
			// indicate that this pass wants this node skipped
			pathVals.push_back( std::make_pair( currPos, 1 ) );
		}
		currPos = prev;
	}

	// perform second pass over the path
	for ( auto pair : pathVals ) {
		std::cout << pair.first.x << " " << pair.first.y << " " << pair.second << std::endl;
	}

	/*
		// start with the 2nd to last node in the path
	int index = pathVals.size() - 2;
	currPos = pathVals[index].first;
	tiles.clear();
	tiles.push_back( pathVals[index + 1].first );
	while ( currPos != end ) {
		glm::ivec2& prev = (glm::ivec2) pathVals[index-1].first;

		// peform path smoothing by not adding points that don't cause the path to go diagonally accross walls
		bool skipPathNode = true;
		// prev up left
		if ( currPos != start && currPos != end && prev != ( glm::ivec2 ) end ) {
			// get the directions of the prev and next nodes
			DIRECTION currToPrev = getDirection( tiles.back(), currPos );
			DIRECTION currToNext = getDirection( ( glm::uvec2 ) gridNodes[currPos.y][currPos.x].prev, currPos );
			// check cases
			if ( ( ( ( isDirection( tiles.back(), currPos, UP ) && currToNext == RIGHT ) || ( isDirection( tiles.back(), currPos, RIGHT ) && currToNext == UP ) ) && grid[currPos.y - 1][currPos.x + 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, DOWN ) && currToNext == RIGHT ) || ( isDirection( tiles.back(), currPos, RIGHT ) && currToNext == DOWN ) ) && grid[currPos.y + 1][currPos.x + 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, UP ) && currToNext == LEFT ) || ( isDirection( tiles.back(), currPos, LEFT ) && currToNext == UP ) ) && grid[currPos.y - 1][currPos.x - 1].taken )
				|| ( ( ( isDirection( tiles.back(), currPos, DOWN ) && currToNext == LEFT ) || ( isDirection( tiles.back(), currPos, LEFT ) && currToNext == DOWN ) ) && grid[currPos.y + 1][currPos.x - 1].taken ) ) {
				skipPathNode = false;
			}
		} else {
			skipPathNode = false;
		}

		if ( !skipPathNode ) {
			tiles.push_back( currPos );
		} else {
			// inicate that this pass wants this node skipped
			pathVals[index].second++;
		}
		// add position to the path
		index--;
		currPos = pathVals[index].first;
	}

	// perform second pass over the path
	std::cout << std::endl;
	for ( auto pair : pathVals ) {
		std::cout << pair.first.x << " " << pair.first.y << " " << pair.second << std::endl;
	}
	*/

	// create the path
	float gridSize = grid[0][1].pos.x - grid[0][0].pos.x;
	for ( int i = pathVals.size()-1; i >= 0; --i ) {
		if ( pathVals[i].second != 1 ) {
			path.push_back( grid[pathVals[i].first.y][pathVals[i].first.x].pos + gridSize/2.0f );
		}
	}
	//std::reverse( path.begin(), path.end() );

	for ( int i = 0; i < grid.size(); ++i ) {
		delete[] gridNodes[i];
		delete[] heuristic[i];
	}
	delete[] gridNodes;
	delete[] heuristic;

	return pathFound;
}

DIRECTION PathSystem::getDirection( glm::uvec2 pos1, glm::uvec2 pos2 ) {
	if ( pos1.x < pos2.x ) {
		return LEFT;
	} else if ( pos1.x > pos2.x ) {
		return RIGHT;
	} else if ( pos1.y < pos2.y ) {
		return UP;
	} else {
		return DOWN;
	}
}

bool PathSystem::isDirection( glm::uvec2 pos1, glm::uvec2 pos2, DIRECTION dir ) {
	switch ( dir ) {
	case UP:
		return pos1.y < pos2.y;
	case DOWN:
		return pos1.y > pos2.y;
	case LEFT:
		return pos1.x < pos2.x;
	case RIGHT:
		return pos1.x > pos2.x;
	}
}