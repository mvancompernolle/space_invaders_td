#include "../includes/Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::setKeyPressed( unsigned int type )
{
	keysPressed[type] = GL_TRUE;
	notifyKeyPress( type );
}

void Input::setKeyProcessed( unsigned int type )
{
	if ( keysPressed[type] ) {
		keysProcessed[type] = GL_TRUE;
	}
}

void Input::setKeyReleased( unsigned int type )
{
	keysPressed[type] = GL_FALSE;
	keysProcessed[type] = GL_FALSE;
	notifyKeyRelease( type );
	notifyRelease( getMousePos() );
}

void Input::setMouseButtonClicked( unsigned int type ) {
	keysPressed[type] = GL_TRUE;
	notifyClick( getMousePos() );
}

GLboolean Input::getKeyPressed( unsigned int type ) const
{
	return keysPressed[type];
}

GLboolean Input::keyNotProcessed( unsigned int type ) const
{
	return !keysProcessed[type];
}

void Input::setMousePos( GLfloat x, GLfloat y ) {
	mouseX = x;
	mouseY = y;
	notifyMovement( getMousePos() );
}

glm::vec2 Input::getMousePos() const {
	return glm::vec2( mouseX * mouseDistortion.x, mouseY * mouseDistortion.y );
}

void Input::scrollOffset( GLfloat x, GLfloat y ) {
	if ( x != 0 ) {
		notifyHorizontalScroll( x, getMousePos() );
	}
	if ( y != 0 ) {
		notifyVerticalScroll( y, getMousePos() );
	}
}

unsigned Input::getKeyIndex( unsigned key ) const {
	return (unsigned) inputMap.at(key);
}

void Input::setMouseDistortion( glm::vec2 dimensions ) {
	mouseDistortion = dimensions;
}