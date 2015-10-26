#pragma once

#ifndef ON_SCROLL_SUBJECT_H
#define ON_SCROLL_SUBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "ScrollObserver.h"

class OnScrollSubject {
public:
	OnScrollSubject();
	~OnScrollSubject();
	void notifyHorizontalScroll( GLfloat offset, glm::vec2 pos );
	void notifyVerticalScroll( GLfloat offset, glm::vec2 pos );
	void addOnScrollObserver( ScrollObserver* observer );
	void removeOnScrollObserver( ScrollObserver* observer );
private:
	std::vector<ScrollObserver*> scrollObservers;
};

#endif ON_SCROLL_SUBJECT_H