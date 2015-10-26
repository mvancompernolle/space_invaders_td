#include "EntityManager.h"
#include <algorithm>


EntityManager::EntityManager() {
}


EntityManager::~EntityManager() {
}

Entity EntityManager::createEntity() {
	Entity ent;
	entities.push_back( ent );
	return ent;
}

void EntityManager::removeEntity( const Entity& ent ) {
	// loop through map and remove all componenets of an entity
	for ( int i = 0; i < ent.getComponents().size(); ++i ) {
		COMPONENT_TYPE type = ent.getComponents()[i];
		std::pair<ComponentIterator, ComponentIterator> it = components.equal_range( type );
		bool found = false;
		for ( ComponentIterator iter = it.first; iter != it.second && !found; ++iter ) {
			if ( iter->second.first == ent.getID() ) {
				components.erase( iter );
				found = true;
			}
		}
	}

	// remove the entity
	entities.erase( std::remove( entities.begin(), entities.end(), ent ), entities.end() );
}

void EntityManager::addComponent( Entity& ent, const Component& component, COMPONENT_TYPE type ) {
	// add component to entity
	components.insert( std::pair<COMPONENT_TYPE, std::pair<unsigned int, Component>>(type, std::make_pair(ent.getID(), component) ) );
	ent.addComponentType( type );
}

Component* EntityManager::getComponent( const Entity ent, COMPONENT_TYPE type ) {
	// point to start of components
	std::pair<ComponentIterator, ComponentIterator> compRange = components.equal_range( type );
	// loop through all componenets of the right type
	for ( ComponentIterator it = compRange.first; it != compRange.second; ++it ) {
		if ( it->second.first == ent.getID() ) {
			// return component if for the correct entity
			return &it->second.second;
		}
	}
	// return null if not found
	return nullptr;
}

std::vector<Component> EntityManager::getEntitiesWithComponent( COMPONENT_TYPE type ) {
	std::vector<Component> comps;

	// point to start of components
	std::pair<ComponentIterator, ComponentIterator> compRange = components.equal_range( type );
	// loop through all componenets of the right type
	for ( ComponentIterator it = compRange.first; it != compRange.second; ++it ) {
		// add all components of that type
		comps.push_back( it->second.second );
	}
	return comps;
}
