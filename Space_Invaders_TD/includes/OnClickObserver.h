#pragma once

#ifndef ON_CLICK_OBSERVER_H
#define ON_CLICK_OBSERVER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class OnClickObserver {
public:
	OnClickObserver();
	virtual ~OnClickObserver();
	virtual void onClick( glm::vec2 pos ) = 0;
	virtual void onRelease( glm::vec2 pos ) = 0;
	virtual void onMouseMovement( glm::vec2 pos ) = 0;
};

#endif // ON_CLICK_OBSERVER_H