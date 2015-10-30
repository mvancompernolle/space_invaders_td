#include "PathSystem.h"
#include "CollisionSystem.h"
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
	if ( entity.path->pathIndex == -1 || glm::distance( entity.world->pos, entity.path->target ) < entity.world->size.x * 0.2f ) {
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
		entity.movement->vel = glm::normalize( entity.path->target - entity.world->pos ) * glm::length( entity.movement->vel );
	}
}

bool PathSystem::calcOptimalPath( glm::uvec2 start, glm::uvec2 end, float radius, const Grid& grid ) {
	GridNode** gridNodes = new GridNode*[grid.size()];
	unsigned** heuristic = new unsigned*[grid.size()];
	for ( int i = 0; i < grid.size(); ++i ) {
		gridNodes[i] = new GridNode[grid[i].size()];
		heuristic[i] = new unsigned[grid[i].size()];

		// init values
		for ( int j = 0; j < grid[i].size(); ++j ) {
			gridNodes[i][j].score = ~0;
			gridNodes[i][j].prev = glm::vec2( -1 );
			gridNodes[i][j].traversed = false;
			gridNodes[i][j].pos = glm::uvec2( j, i );
		}
	}

	// calculate heuristic for each spot on grid
	for ( int i = 0; i < grid.size(); ++i ) {
		for ( int j = 0; j < grid.size(); ++j ) {
			heuristic[i][j] = std::abs( (int)( j - end.x ) ) + std::abs( (int)( i - end.y ) );
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
		if ( node.pos.y < grid.size() - 1 && !grid[node.pos.y + 1][node.pos.x].taken && !gridNodes[node.pos.y + 1][node.pos.x].traversed ) {
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
				gridNodes[node.pos.y][node.pos.x - 1].score = node.score + 1;
				gridNodes[node.pos.y][node.pos.x - 1].prev = node.pos;
				open.push( gridNodes[node.pos.y][node.pos.x - 1] );
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

	glm::ivec2 currPos = (glm::ivec2) end;
	std::vector<glm::uvec2> addedTiles, allTiles;
	while ( currPos != glm::ivec2(-1) ) {
		glm::ivec2& next = gridNodes[currPos.y][currPos.x].prev;

		if ( ( glm::uvec2 ) currPos != end ) {
			// check to see if the currPos intersects a wall between the last position
			if ( ( currPos.x != addedTiles.back().x && currPos.y != addedTiles.back().y ) && isCollisionBetween( ( glm::uvec2 ) currPos, addedTiles.back(), radius, grid ) ) {
				// if there is, add the previous tile
				addedTiles.push_back( allTiles.back() );
			}
		} else {
			// if first or last nodes, add to current trail
			addedTiles.push_back( ( glm::uvec2 ) currPos );
		}

		allTiles.push_back( ( glm::uvec2 ) currPos );
		currPos = next;
	}
	if ( allTiles.back() != start ) {
		return false;
	}

	// create the path
	float gridSize = grid[0][1].pos.x - grid[0][0].pos.x;
	for ( int i = addedTiles.size() - 1; i >= 0; --i ) {
		path.push_back( grid[addedTiles[i].y][addedTiles[i].x].pos + gridSize / 2.0f );
	}

	for ( int i = 0; i < grid.size(); ++i ) {
		delete[] gridNodes[i];
		delete[] heuristic[i];
	}
	delete[] gridNodes;
	delete[] heuristic;

	return true;
}

bool PathSystem::isCollisionBetween( glm::uvec2 curr, glm::uvec2 prev, float radius, const Grid& grid ) {
	// create a rect representing the straight path between the 2 points
	float gridSize = grid[0][1].pos.x - grid[0][0].pos.x;
	float width = radius / gridSize;
	glm::vec3 perp = glm::normalize( glm::cross( glm::vec3( curr, 0.0f ) - glm::vec3( prev, 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) ) * width;
	std::vector<glm::vec2> pathRect;
	pathRect.push_back( ( glm::vec2 ) curr - glm::vec2( perp ) + 0.5f );
	pathRect.push_back( ( glm::vec2 ) curr + glm::vec2( perp ) + 0.5f );
	pathRect.push_back( ( glm::vec2 ) prev + glm::vec2( perp ) + 0.5f );
	pathRect.push_back( ( glm::vec2 ) prev - glm::vec2( perp ) + 0.5f );

	// loop over all tiles in range
	int minX = std::min( curr.x, prev.x );
	int minY = std::min( curr.y, prev.y );
	int maxX = std::max( curr.x, prev.x );
	int maxY = std::max( curr.y, prev.y );
	for ( int i = minY; i <= maxY; ++i ) {
		for ( int j = minX; j <= maxX; ++j ) {
			// if tile is taken, check for a collision
			if ( grid[i][j].taken ) {
				std::vector<glm::vec2> tile;
				tile.push_back( glm::vec2( j, i ) );
				tile.push_back( glm::vec2( j + 1, i ) );
				tile.push_back( glm::vec2( j + 1, i + 1) );
				tile.push_back( glm::vec2( j, i + 1) );
				if ( CollisionSystem::arePolygonsIntersecting( pathRect, tile ) ) {
					return true;
				}
			}
		}
	}
	return false;
}