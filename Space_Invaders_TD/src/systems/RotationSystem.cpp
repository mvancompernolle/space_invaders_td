#include "systems/RotationSystem.h"



RotationSystem::RotationSystem( unsigned* enemiesLeft ) : System( enemiesLeft )
{
	flags = ( WORLD | ROTATION );
	systemName = "rotSystem";
}


RotationSystem::~RotationSystem()
{
}

void RotationSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	RotationComponent& rotComp = world->rotationComponents[world->getComponentIndex( pos, ROTATION )];
	// rotate the entity
	worldComp.rotation += rotComp.rotSpeed * dt;
}
