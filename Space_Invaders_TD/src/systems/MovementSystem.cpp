#include "systems/MovementSystem.h"
#include "Consts.h"

MovementSystem::MovementSystem() {
	flags = MOVEMENT | WORLD;
}


MovementSystem::~MovementSystem() {
}

void MovementSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	MovementComponent& movComp = world->movementComponents[world->getComponentIndex( pos, MOVEMENT )];
	worldComp.pos += movComp.vel * dt;

	// remove entity if it goes off screen
	if ( worldComp.pos.x < -worldComp.size.x || worldComp.pos.x > GAME_WIDTH || worldComp.pos.y < -worldComp.size.y ||
		worldComp.pos.y > GAME_HEIGHT ) {
		removals.push_back( pos );
	}
}
