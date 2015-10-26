#include "../includes/Graphics.h"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

glm::vec2 Graphics::getDimensions() const {
	return glm::vec2( windowWidth, windowHeight );
}
