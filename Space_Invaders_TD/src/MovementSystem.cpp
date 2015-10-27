#include "MovementSystem.h"

MovementSystem::MovementSystem() {
	flags = MOVEMENT | WORLD;
}


MovementSystem::~MovementSystem() {
}

void MovementSystem::update( const Entity& entity, float dt ) {
	entity.world->pos += entity.movement->vel * dt;
}
