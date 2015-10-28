#include "Entity.h"

unsigned Entity::ID;

Entity::Entity() : health(nullptr), render(nullptr), world(nullptr) {
	myID = ID++;
}

Entity::Entity( unsigned components ) {
	myID = ID++;
	componentTypes = components;

	health = ( 0 | HEALTH ) & components ? new HealthComponent : nullptr;
	world = ( 0 | WORLD ) & components ? new WorldComponent : nullptr;
	render = ( 0 | RENDER ) & components ? new RenderComponent : nullptr;
	movement = ( 0 | MOVEMENT ) & components ? new MovementComponent : nullptr;
	path = ( 0 | PATH ) & components ? new PathAIComponent : nullptr;
	spawn = ( 0 | SPAWN ) & components ? new SpawnComponent : nullptr;
}

Entity::Entity( const Entity& entity ) 
	: health(nullptr), world(nullptr), render(nullptr), movement(nullptr), path(nullptr), spawn(nullptr) {
	myID = entity.myID;
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
	if ( entity.path != nullptr ) {
		path = new PathAIComponent;
		path->target = entity.path->target;
		path->pathIndex = entity.path->pathIndex;
	}
	if ( entity.spawn != nullptr ) {
		spawn = new SpawnComponent;
		spawn->spawnRate = entity.spawn->spawnRate;
		spawn->dt = entity.spawn->dt;
		spawn->round = entity.spawn->round;
		spawn->numRounds = entity.spawn->numRounds;
		spawn->currSpawnNum = entity.spawn->currSpawnNum;
		spawn->spawnTypes = entity.spawn->spawnTypes;
	}
}

Entity& Entity::operator=( const Entity& entity ) {
	if ( this != &entity ) {
		myID = entity.myID;
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
		if ( entity.path != nullptr ) {
			if ( path == nullptr ) { path = new PathAIComponent; }
			path->target = entity.path->target;
			path->pathIndex = entity.path->pathIndex;
		}
		if ( entity.spawn != nullptr ) {
			if ( spawn == nullptr ) { spawn = new SpawnComponent; }
			spawn->spawnRate = entity.spawn->spawnRate;
			spawn->dt = entity.spawn->dt;
			spawn->round = entity.spawn->round;
			spawn->numRounds = entity.spawn->numRounds;
			spawn->currSpawnNum = entity.spawn->currSpawnNum;
			spawn->spawnTypes = entity.spawn->spawnTypes;
		}
	}
	return *this;
}

bool Entity::operator==( const Entity& entity ) {
	return ( myID == entity.myID );
}

Entity::~Entity() {
	if ( health != nullptr ) { delete health; }
	if ( world != nullptr ) { delete world; }
	if ( render != nullptr ) { delete render; }
	if ( movement != nullptr ) { delete movement; }
	if ( path != nullptr ) { delete path; }
	if ( spawn != nullptr ) { delete spawn; }
}
