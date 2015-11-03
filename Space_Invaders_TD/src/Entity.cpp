#include "Entity.h"

Entity::Entity( unsigned components ) {
	componentTypes = components;
	health = ( 0 | HEALTH ) & components ? new HealthComponent : nullptr;
	world = ( 0 | WORLD ) & components ? new WorldComponent : nullptr;
	render = ( 0 | RENDER ) & components ? new RenderComponent : nullptr;
	movement = ( 0 | MOVEMENT ) & components ? new MovementComponent : nullptr;
	path = ( 0 | PATH ) & components ? new PathAIComponent : nullptr;
	spawn = ( 0 | SPAWN ) & components ? new SpawnComponent : nullptr;
	keyboard = ( 0 | KEYBOARD ) & components ? new KeyboardInputComponent(this) : nullptr;
}

Entity::~Entity() {
	if ( health != nullptr ) { delete health; }
	if ( world != nullptr ) { delete world; }
	if ( render != nullptr ) { delete render; }
	if ( movement != nullptr ) { delete movement; }
	if ( path != nullptr ) { delete path; }
	if ( spawn != nullptr ) { delete spawn; }
	if ( keyboard != nullptr ) { 
		delete keyboard; 
	}
}
