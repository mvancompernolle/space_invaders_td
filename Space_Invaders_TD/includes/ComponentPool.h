#pragma once

#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include "Component.h"
#include <iostream>
#include <string>

template<typename T>
class Component {
public:
	T data;
	Component<T>* next;
	Component<T>* getNext() const { return next; }
	void setNext( Component<T>* n ) { next = n; }
private:
};


template<typename T, int S>
class ComponentPool {
public:
	ComponentPool( std::string name ) : name(name)  {
		// create component array
		size = 0;
		components = new Component<T>[S];

		// set the first component to available
		firstAvailable = components;

		// each component points to the next
		for ( int i = 0; i < S - 1; i++ ) {
			components[i].setNext( &components[i+1] );
		}

		components[S - 1].setNext( nullptr );
	}

	~ComponentPool() { delete[] components; }

	int create() {
		// make sure the pool isn't full
		assert( firstAvailable != nullptr && "NO MORE COMPONENTS AVAILABLE" );

		// remove it from the available list
		Component<T> *newComponent = firstAvailable;
		firstAvailable = newComponent->getNext();
		size++;
		//std::cout << name << " creating - size: " << size << std::endl;

		// set next to null to indicate that it is taken
		//newComponent->setNext( nullptr );

		ptrdiff_t index = newComponent - components;
		return index;
	}

	void remove( int i ) {
		//if ( components[i].getNext() == nullptr ) {
		// add this particle to the front of the list
		components[i].setNext( firstAvailable );
		firstAvailable = &components[i];

		size--;
		//std::cout << name << " removing - size: " << size << std::endl;
		//}
	}

	T& operator[]( int pos ) {
		assert( pos < S && pos > -1 && "Component access out of bounds" );
		return components[pos].data;
	}

	const T& operator[]( int pos ) const {
		assert( pos < S && pos > -1 && "Component access out of bounds" );
		return components[pos].data;
	}

private:
	Component<T> *firstAvailable;
	Component<T> *components;
	int size;
	std::string name;
};


#endif COMPONENT_POOL_H
