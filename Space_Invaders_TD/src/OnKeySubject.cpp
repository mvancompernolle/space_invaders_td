#include "OnKeySubject.h"
#include <algorithm>


OnKeySubject::OnKeySubject()
{
}


OnKeySubject::~OnKeySubject()
{
}

void OnKeySubject::notifyKeyPress( unsigned key ) {
	for ( OnKeyObserver*& observer : keyObservers ) {
		observer->onKeyPressed( key );
	}
}

void OnKeySubject::notifyKeyRelease( unsigned key ) {
	for ( OnKeyObserver*& observer : keyObservers ) {
		observer->onKeyReleased( key );
	}
}

void OnKeySubject::addOnKeyObserver( OnKeyObserver* observer ) {
	keyObservers.push_back( observer );
}

void OnKeySubject::removeOnKeyObserver( OnKeyObserver* observer ) {
	keyObservers.erase( std::remove( keyObservers.begin(), keyObservers.end(), observer ), keyObservers.end() );
}
