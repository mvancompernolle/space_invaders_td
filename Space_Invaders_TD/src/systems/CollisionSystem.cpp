#include "systems/CollisionSystem.h"
#include "Consts.h"
#include <algorithm>

CollisionSystem::CollisionSystem()
{
	flags = ( COLLISION | WORLD );
}


CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::clearCollisionEvents() {
	collisions.clear();
}

void CollisionSystem::registerEntity( unsigned entity ) {
	registeredEntities.push_back( entity );
}

void CollisionSystem::unregisterEntity( unsigned entity ) {
	registeredEntities.erase( std::remove( registeredEntities.begin(), registeredEntities.end(), entity ), registeredEntities.end() );
}

void CollisionSystem::update( World* world ) {

	for ( int i = 0; i < (int) registeredEntities.size() - 1; ++i ) {
		for ( int j = i + 1; j < (int) registeredEntities.size(); ++j ) {
			CollisionComponent& collComp1 = world->collisionComponents[world->getComponentIndex( registeredEntities[i], COLLISION )];
			CollisionComponent& collComp2 = world->collisionComponents[world->getComponentIndex( registeredEntities[j], COLLISION )];

			// check to see if the two objects collide
			// make sure both entities have correct components and are set to check for collisions with eachother
			if ( !( ( flags ^ world->entities[registeredEntities[i]].mask ) & flags ) && !( ( flags ^ world->entities[registeredEntities[j]].mask ) & flags ) 
				&& (collComp1.collisionMask & collComp2.collisionID) ) {
				WorldComponent& worldComp1 = world->worldComponents[world->getComponentIndex( registeredEntities[i], WORLD )];
				WorldComponent& worldComp2 = world->worldComponents[world->getComponentIndex( registeredEntities[j], WORLD )];

				// check collision based on collision type
				bool wasCollision = false;
				unsigned shapes = ( collComp1.shape | collComp2.shape );
				if ( shapes == RECTANGLE ) {
					wasCollision = areRectsIntersecting( worldComp1.pos, worldComp2.pos, worldComp1.size, worldComp2.size );
				} else if ( shapes == CIRCLE ) {
					float size1 = ( worldComp1.size.x * collComp1.collisionScale ) / 2.0f;
					float size2 = ( worldComp2.size.x * collComp2.collisionScale ) / 2.0f;
					wasCollision = areCirclesIntersecting( getCenter(worldComp1.pos, worldComp1.size),
						getCenter( worldComp2.pos, worldComp2.size ), size1, size2);
				} else if ( shapes == ( RECTANGLE | CIRCLE ) ) {
					if ( collComp1.shape == RECTANGLE ) {
						float size = ( worldComp2.size.x * collComp2.collisionScale ) / 2.0f;
						wasCollision = areRectCircleIntersecting( worldComp1.pos, getCenter( worldComp2.pos, worldComp2.size ), worldComp1.size, size,
							worldComp1.rotation );
					} else {
						float size = ( worldComp1.size.x * collComp1.collisionScale ) / 2.0f;
						wasCollision = areRectCircleIntersecting( worldComp2.pos, getCenter( worldComp1.pos, worldComp1.size ), worldComp2.size, size, worldComp2.rotation );
					}
				}

				// add collision event if there was one
				if ( wasCollision ) {
					createCollisionEvents( *world, registeredEntities[i], registeredEntities[j] );
				}
			}
		}
	}
}

glm::vec2 CollisionSystem::getCenter( glm::vec2 pos, glm::vec2 size ) const {
	return pos + size * 0.5f;
}

bool CollisionSystem::arePolygonsIntersecting( std::vector<glm::vec2> p1, std::vector<glm::vec2> p2 ) {
	std::vector<glm::vec2> arr[2] = { p1, p2 };
	// check for each polygon
	for ( int i = 0; i < 2; ++i ) {

		std::vector<glm::vec2>& polygon = arr[i];
		for ( int i1 = 0; i1 < polygon.size(); ++i1 ) {
			// grab 2 vertices to create an edge
			int i2 = ( i1 + 1 ) % polygon.size();
			glm::vec2& v1 = polygon[i1];
			glm::vec2& v2 = polygon[i2];

			// find the line perpendicular to this edge
			glm::vec2 normal = glm::vec2( v2.y - v1.y, v1.x - v2.x );

			// get the min and max on projected line
			float min1, max1, min2, max2, dotProduct;
			dotProduct = normal.x * p1[0].x + normal.y * p1[0].y;
			min1 = max1 = dotProduct;
			// for each vertex in the first shape, project it onto the line perpendicular to the edge
			// and keep track of the min and max of these values
			for ( int j = 1; j < p1.size(); ++j ) {
				dotProduct = normal.x * p1[j].x + normal.y * p1[j].y;
				if ( dotProduct < min1 ) {
					min1 = dotProduct;
				}
				if ( dotProduct > max1 ) {
					max1 = dotProduct;
				}
			}

			// for each vertex in the first shape, project it onto the line perpendicular to the edge
			// and keep track of the min and max of these values
			dotProduct = normal.x * p2[0].x + normal.y * p2[0].y;
			min2 = max2 = dotProduct;
			for ( int j = 1; j < p2.size(); ++j ) {
				dotProduct = normal.x * p2[j].x + normal.y * p2[j].y;
				if ( dotProduct < min2 ) {
					min2 = dotProduct;
				}
				if ( dotProduct > max2 ) {
					max2 = dotProduct;
				}
			}

			// test to see if they are not overlapping
			if ( !( min2 <= max1 && max2 >= min1 ) ) {
				return false;
			}
		}

	}
	return true;
}

bool CollisionSystem::areRectsIntersecting( glm::vec2 pos1, glm::vec2 pos2, glm::vec2 size1, glm::vec2 size2 ) {
	return ( pos1.x + size1.x >= pos2.x ) && ( pos2.x + size2.x >= pos1.x )
		&& ( pos1.y + size1.y >= pos2.y ) && ( pos2.y + size2.y >= pos1.y );
}

bool CollisionSystem::areCirclesIntersecting( glm::vec2 pos1, glm::vec2 pos2, float radius1, float radius2 ) {
	return glm::distance( pos1, pos2 ) <= radius1 + radius2;
}

bool CollisionSystem::areRectCircleIntersecting( glm::vec2 rectPos, glm::vec2 cirPos, glm::vec2 rectSize, float cirRadius, float rectRot ) {
	glm::vec2 circlePos = cirPos;
	if ( rectRot != 0.0f ) {
		// if rectangle is rotated, rotate circle with respect to the rectangles origin center
		GLfloat angle = -rectRot * ( PI / 180.0f );
		GLfloat unrotatedCircleX = std::cos( angle ) * ( cirPos.x - rectPos.x ) - std::sin( angle ) * ( cirPos.y - rectPos.y ) + rectPos.x;
		GLfloat unrotatedCircleY = std::sin( angle ) * ( cirPos.x - rectPos.x ) + std::cos( angle ) * ( cirPos.y - rectPos.y ) + rectPos.y;
		circlePos = glm::vec2( unrotatedCircleX, unrotatedCircleY ) - cirRadius;
	}

	// calculate center of ball and rect
	glm::vec2 centerBall( circlePos + cirRadius );
	glm::vec2 rectHalfExtents( rectSize.x / 2, rectSize.y / 2 );
	glm::vec2 centerRect( rectPos.x + rectHalfExtents.x, rectPos.y + rectHalfExtents.y );

	// get direction between ball and rect and clamp it to edge of rectangle
	glm::vec2 direction = centerBall - centerRect;
	glm::vec2 clamped = glm::clamp( direction, -rectHalfExtents, rectHalfExtents );

	// find the closest point of rect and se if it is in circle's radius
	glm::vec2 closest = centerRect + clamped;
	direction = closest - centerBall;

	return glm::length( direction ) <= cirRadius ;
}

void CollisionSystem::createCollisionEvents( const World& world, unsigned ent1, unsigned ent2 ) {
	int type1 = world.collisionComponents[world.getComponentIndex(ent1, COLLISION)].collisionID, 
		type2 = world.collisionComponents[world.getComponentIndex( ent2, COLLISION )].collisionID;
	if ( ( ( type1 | type2 ) & BULLET ) && ( ( type1 | type2 ) & ENEMY ) ) {
		collisions.push_back( CollisionEvent( ent1, ent2, DAMAGE_EVENT ) );
	}
	if ( ( ( type1 | type2 ) & DESPAWN ) && ( ( type1 | type2 ) & ENEMY ) ) {
		collisions.push_back( CollisionEvent( ent1, ent2, DESPAWN_EVENT ) );
	}
}