#pragma once

#ifndef ON_KEY_SUBJECT_H
#define ON_KEY_SUBJECT_H

#include <vector>
#include <glm/glm.hpp>
#include "OnKeyObserver.h"

class OnKeySubject {
public:
	OnKeySubject();
	~OnKeySubject();
	void notifyKeyPress( unsigned key );
	void notifyKeyRelease( unsigned key );
	void addOnKeyObserver( OnKeyObserver* observer );
	void removeOnKeyObserver( OnKeyObserver* observer );
private:
	std::vector<OnKeyObserver*> keyObservers;
};

#endif // ON_KEY_SUBJECT_H

