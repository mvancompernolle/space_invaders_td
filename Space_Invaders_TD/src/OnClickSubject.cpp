#include "../includes/OnClickSubject.h"
#include <algorithm>


OnClickSubject::OnClickSubject()
{
}


OnClickSubject::~OnClickSubject()
{
}

void OnClickSubject::notifyClick( glm::vec2 pos ) {
	if ( observers.size() > 0 ) {
		for ( OnClickObserver*& observer : observers ) {
			observer->onClick( pos );
		}
	}
}

void OnClickSubject::notifyRelease( glm::vec2 pos ) {
	for ( OnClickObserver*& observer : observers ) {
		observer->onRelease( pos );
	}
}

void OnClickSubject::notifyMovement( glm::vec2 pos ) {
	for ( OnClickObserver*& observer : observers ) {
		observer->onMouseMovement( pos );
	}
}

void OnClickSubject::addOnClickObserver( OnClickObserver* observer ) {
	observers.push_back( observer );
}

void OnClickSubject::removeOnClickObserver( OnClickObserver* observer ) {
	observers.erase( std::remove( observers.begin(), observers.end(), observer ), observers.end() );
}