#pragma once

#ifndef ON_CLICK_SUBJECT_H
#define ON_CLICK_SUBJECT_H

#include <vector>
#include <glm/glm.hpp>
#include "OnClickObserver.h"

class OnClickSubject {
public:
	OnClickSubject();
	~OnClickSubject();
	void notifyClick( glm::vec2 pos );
	void notifyRelease( glm::vec2 pos );
	void notifyMovement( glm::vec2 pos );
	void addOnClickObserver( OnClickObserver* observer );
	void removeOnClickObserver( OnClickObserver* observer );
private:
	std::vector<OnClickObserver*> observers;
};

#endif //ON_CLICK_SUBJECT_H