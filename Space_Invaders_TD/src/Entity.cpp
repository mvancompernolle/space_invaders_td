#include "Entity.h"

Entity::Entity() : health(nullptr), render(nullptr), world(nullptr) {
}

Entity::Entity( unsigned components ) {
	componentTypes = components;

	health = ( 0 | HEALTH ) & components ? new HealthComponent : nullptr;
	world = ( 0 | WORLD ) & components ? new WorldComponent : nullptr;
	render = ( 0 | RENDER ) & components ? new RenderComponent : nullptr;
	movement = ( 0 | MOVEMENT ) & components ? new MovementComponent : nullptr;
	ai = ( 0 | AI ) & components ? new AIComponent : nullptr;
}

Entity::Entity( const Entity& entity ) 
	: health(nullptr), world(nullptr), render(nullptr), movement(nullptr), ai(nullptr) {
	componentTypes = entity.componentTypes;

	if ( entity.health != nullptr ) { 
		health = new HealthComponent; 
		health->currHP = entity.health->currHP;
		health->maxHP = entity.health->maxHP;
	}
	if ( entity.world != nullptr ) {
		world = new WorldComponent;
		world->pos = entity.world->pos;
		world->rotation = entity.world->rotation;
		world->size = entity.world->size;
		world->shape = entity.world->shape;
	}
	if ( entity.render != nullptr ) {
		render = new RenderComponent;
		render->textureName = entity.render->textureName;
		render->color = entity.render->color;
	}
	if ( entity.movement != nullptr ) {
		movement = new MovementComponent;
		movement->vel = entity.movement->vel;
		movement->defSpeed = entity.movement->defSpeed;
	}
	if ( entity.ai != nullptr ) {
		ai = new AIComponent;
		ai->target = entity.ai->target;
		ai->pathIndex = entity.ai->pathIndex;
		ai->AIType = entity.ai->AIType;
	}
}

Entity& Entity::operator=( const Entity& entity ) {
	if ( this != &entity ) {
		componentTypes = entity.componentTypes;

		if ( entity.health != nullptr ) {
			if ( health == nullptr ) { health = new HealthComponent; }
			health->currHP = entity.health->currHP;
			health->maxHP = entity.health->maxHP;
		}
		if ( entity.world != nullptr ) {
			if ( world == nullptr ) { world = new WorldComponent; }
			world->pos = entity.world->pos;
			world->rotation = entity.world->rotation;
			world->size = entity.world->size;
			world->shape = entity.world->shape;
		}
		if ( entity.render != nullptr ) {
			if ( render == nullptr ) { render = new RenderComponent; }
			render->textureName = entity.render->textureName;
			render->color = entity.render->color;
		}
		if ( entity.movement != nullptr ) {
			if ( movement == nullptr ) { movement = new MovementComponent;  }
			movement->vel = entity.movement->vel;
			movement->defSpeed = entity.movement->defSpeed;
		}
		if ( entity.ai != nullptr ) {
			if ( ai == nullptr ) { ai = new AIComponent; }
			ai->target = entity.ai->target;
			ai->AIType = entity.ai->AIType;
			ai->pathIndex = entity.ai->pathIndex;
		}
	}
	return *this;
}

Entity::~Entity() {
	if ( health != nullptr ) { delete health; }
	if ( world != nullptr ) { delete world; }
	if ( render != nullptr ) { delete render; }
	if ( movement != nullptr ) { delete movement; }
	if ( ai != nullptr ) { delete ai; }
}
