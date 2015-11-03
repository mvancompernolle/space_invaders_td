#include "CollisionSystem.h"



CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::update( World* world, int pos, float dt ) {

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
