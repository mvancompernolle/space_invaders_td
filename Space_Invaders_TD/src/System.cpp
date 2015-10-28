#include "System.h"
#include <algorithm>


System::System( ) {
}


System::~System() {
}

bool System::condition( unsigned componentTypes ){
	return !( ( flags ^ componentTypes ) & flags );
}

void System::adjustEntityVector( std::vector<Entity>& src ) {
	// remove anything the system was supposed to remove
	for ( int i = 0; i < removals.size(); ++i ) {
		src.erase( std::remove( src.begin(), src.end(), removals[i] ), src.end() );
	}
	// add anything the system was supposed to add
	src.insert( src.end(), additions.begin(), additions.end() );

	additions.clear();
	removals.clear();
}