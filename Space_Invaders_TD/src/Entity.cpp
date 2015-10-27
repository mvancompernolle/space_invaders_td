#include "Entity.h"

Entity::Entity() : health(nullptr), render(nullptr), world(nullptr) {
}

Entity::Entity( unsigned components ) {
	componentTypes = components;

	health = ( 0 | HEALTH ) & components ? new HealthComponent : nullptr;
	world = ( 0 | WORLD ) & components ? new WorldComponent : nullptr;
	render = ( 0 | RENDER ) & components ? new RenderComponent : nullptr;
	movement = ( 0 | MOVEMENT ) & components ? new MovementComponent : nullptr;
}

Entity::~Entity() {
}
