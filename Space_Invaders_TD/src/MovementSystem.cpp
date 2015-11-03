#include "MovementSystem.h"

MovementSystem::MovementSystem() {
	flags = MOVEMENT | WORLD;
}


MovementSystem::~MovementSystem() {
}

void MovementSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	MovementComponent& movComp = world->movementComponents[world->getComponentIndex( pos, MOVEMENT )];
	worldComp.pos += movComp.vel * dt;
}
