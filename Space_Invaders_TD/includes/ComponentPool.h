#pragma once

#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include "Component.h"
#include <iostream>

template<typename T>
class Component {
public:
	T data;
	Component<T>* getNext() const { return next; }
	void setNext( Component<T>* n ) { next = n; }
private:
	Component<T>* next;
};


template<typename T, int S>
class ComponentPool {
public:
	ComponentPool() {
		// create component array
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

		ptrdiff_t index = newComponent - components;
		return index;
	}

	void remove( int i ) {
		// add this particle to the front of the list
		components[i].setNext( firstAvailable );
		firstAvailable = &components[i];
	}

	T& operator[]( int pos ) {
		return components[pos].data;
	}

private:
	Component<T> *firstAvailable;
	Component<T> *components;
};


#endif COMPONENT_POOL_H
