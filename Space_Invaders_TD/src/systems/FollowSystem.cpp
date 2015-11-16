#include "systems/FollowSystem.h"



FollowSystem::FollowSystem() {
	flags = ( MOVEMENT | WORLD | FOLLOW );
}


FollowSystem::~FollowSystem() {
}

void FollowSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	MovementComponent& moveComp = world->movementComponents[world->getComponentIndex( pos, MOVEMENT )];
	FollowComponent& followComp = world->followComponents[world->getComponentIndex( pos, FOLLOW )];

	// follow target if set
	if ( followComp.entTarget != -1 ) {
		if ( world->entities[followComp.entTarget].mask != NONE ) {
			WorldComponent& worldComp2 = world->worldComponents[world->getComponentIndex( followComp.entTarget, WORLD )];

			// get desired direction to target
			glm::vec2 posDiff = worldComp2.getCenter() - worldComp.getCenter();
			float desiredRotation = glm::degrees( std::atan2( posDiff.y, posDiff.x ) );
			if ( desiredRotation < 0 ) {
				desiredRotation += 360.0f;
			}
			float rotation = glm::degrees( std::atan2( moveComp.vel.y, moveComp.vel.x ) );
			if ( rotation < 0 ) {
				rotation += 360.0f;
			}

			if ( std::abs( desiredRotation - rotation ) < followComp.turnRate * dt ) {
				rotation = desiredRotation;
			} else {
				if ( ( desiredRotation > rotation && desiredRotation - rotation <= 180 ) || ( desiredRotation < rotation && rotation - desiredRotation >= 180 ) ) {
					rotation += followComp.turnRate * dt;
				} else {
					rotation -= followComp.turnRate * dt;
				}

				// keep rotation is correct range
				if ( rotation < 0 ) {
					rotation += 360.0f;
				} else if ( rotation > 360.0f ) {
					rotation -= 360.0f;
				}
			}

			// set velocity to move toward target
			float rotInRadians = glm::radians( -rotation );
			float speed = glm::length( moveComp.vel );
			moveComp.vel.x = speed * std::cos( rotInRadians );
			moveComp.vel.y = speed * -std::sin( rotInRadians );
		} else {
			followComp.entTarget = -1;
		}
	}
}