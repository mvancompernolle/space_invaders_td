#pragma once

#ifndef SCROLL_OBSERVER_H
#define SCROLL_OBSERVER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class ScrollObserver {
public:
	ScrollObserver();
	~ScrollObserver();
	virtual void onHorizontalScroll( GLfloat offset, glm::vec2 pos ) = 0;
	virtual void onVerticalScroll( GLfloat offset, glm::vec2 pos ) = 0;
};

#endif // SCROLL_OBSERVER_H
