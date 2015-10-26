#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

class Entity {
public:
	Entity();
	~Entity();
	unsigned long getID() const;
	void addComponentType( COMPONENT_TYPE type );
	const std::vector<COMPONENT_TYPE>& getComponents() const;
	bool operator==( const Entity& rhs );
private:
	static unsigned long currID;
	unsigned long ID;
	std::vector<COMPONENT_TYPE> componentTypes;
};

#endif // ENTITY_H
