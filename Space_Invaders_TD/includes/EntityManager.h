#pragma once

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"
#include "Component.h"
#include <vector>
#include <map>
#include <string>

//typedef std::map<std::pair<unsigned int, COMPONENT_TYPE>, Component> ComponentMap;
typedef std::multimap<COMPONENT_TYPE, std::pair<unsigned int, Component>> ComponentMap;
typedef ComponentMap::iterator ComponentIterator;

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	Entity createEntity();
	void removeEntity( const Entity& ent );
	void addComponent( Entity& ent, const Component& component, COMPONENT_TYPE type );
	Component* getComponent( const Entity ent, COMPONENT_TYPE type );
	std::vector<Component> getEntitiesWithComponent( COMPONENT_TYPE type );
private:
	std::vector<Entity> entities;
	ComponentMap components;
};

#endif // ENTITY_MANAGER_H