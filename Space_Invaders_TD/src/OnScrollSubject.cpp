#include "../includes/OnScrollSubject.h"
#include <algorithm>


OnScrollSubject::OnScrollSubject() {
}


OnScrollSubject::~OnScrollSubject() {
}

void OnScrollSubject::notifyHorizontalScroll( GLfloat offset, glm::vec2 pos ) {
	for ( ScrollObserver*& observer : scrollObservers ) {
		observer->onHorizontalScroll( offset, pos );
	}
}

void OnScrollSubject::notifyVerticalScroll( GLfloat offset, glm::vec2 pos ) {
	for ( ScrollObserver*& observer : scrollObservers ) {
		observer->onVerticalScroll( offset, pos );
	}
}

void OnScrollSubject::addOnScrollObserver( ScrollObserver* observer ) {
	scrollObservers.push_back( observer );
}

void OnScrollSubject::removeOnScrollObserver( ScrollObserver* observer ) {
	scrollObservers.erase( std::remove( scrollObservers.begin(), scrollObservers.end(), observer ), scrollObservers.end() );
}