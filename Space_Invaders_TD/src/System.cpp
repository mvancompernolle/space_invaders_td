#include "System.h"



System::System( ) {
}


System::~System() {
}

bool System::condition( unsigned componentTypes ){
	return !( ( flags ^ componentTypes ) & flags );
}