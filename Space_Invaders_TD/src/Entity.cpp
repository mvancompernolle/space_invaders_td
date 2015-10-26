#include "Entity.h"

unsigned long Entity::currID = 0;

Entity::Entity() : ID(currID++) {
}


Entity::~Entity() {
}

unsigned long Entity::getID() const {
	return ID;
}

const std::vector<COMPONENT_TYPE>& Entity::getComponents() const {
	return componentTypes;
}

bool Entity::operator==( const Entity& rhs ) {
	return ID == rhs.getID();
}

void Entity::addComponentType( COMPONENT_TYPE type ) {
	componentTypes.push_back( type );
}
