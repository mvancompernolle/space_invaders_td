#include "EntityFactory.h"



EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

std::vector<Entity>& EntityFactory::getData() {
	static std::vector<Entity> entities;
	return entities;
}

void EntityFactory::createEnemy() {
	Entity ent( HEALTH | RENDER | WORLD | MOVEMENT );
}

void EntityFactory::createPlayer() {
	Entity ent( RENDER | WORLD | MOVEMENT );
}