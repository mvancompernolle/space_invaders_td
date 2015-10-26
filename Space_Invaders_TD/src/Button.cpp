#include "Button.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include <iostream>


Button::Button( glm::vec2 pos, glm::vec2 size )
	: pos( pos ), size( size ), text( "" ), fontScale( 1.0f ), textColor( glm::vec3( 0.0f ) ),
	state( RELEASED ), rotation( 0.0f ), isDraggable( GL_FALSE ), isVisible( GL_TRUE )
{
	pressedTexture = ResourceManager::loadTexture( "button_pressed.png", GL_TRUE, "btn_pressed_default" );
	releasedTexture = ResourceManager::loadTexture( "button_released.png", GL_TRUE, "btn_released_default" );
	hoverTexture = ResourceManager::loadTexture( "button_hover.png", GL_TRUE, "btn_hover_default" );
	fontType = ResourceManager::getFont( "default" );
}

Button::~Button()
{
}

void Button::onClick( glm::vec2 pos ) {
	// check to see if button was click
	if ( state != DISABLED && isOverButton(pos) ) {
		state = PRESSED;

		if ( onClickFunction ) {
			ServiceLocator::getAudio().playSound( ResourceManager::getPath( "sounds" ) + "button_pressed.wav", GL_FALSE );
			onClickFunction();
		}
	}
}

void Button::onRelease( glm::vec2 pos ) {
	// perform release function if released over the button
	if ( state != DISABLED ) {
		if ( isOverButton( pos ) ) {
			state = HOVER;

			if ( onReleaseFunction ) {
				onReleaseFunction();
			}
		} else {
			state = RELEASED;
		}
	}
}

void Button::onMouseMovement( glm::vec2 pos ) {
	if ( state != DISABLED ) {
		if ( state != PRESSED ) {

			if ( isOverButton( pos ) ) {
				state = HOVER;
			} else {
				state = RELEASED;
			}
		} else {
			if ( isDraggable && onMouseMovementFunction ) {
				onMouseMovementFunction();
			}
		}
	}
}

void Button::setTextures( Texture pressed, Texture released, Texture hover ) {
	pressedTexture = pressed;
	releasedTexture = released;
	hoverTexture = hover;
}

void Button::setTextColor( glm::vec3 color ) {
	textColor = color;
}

void Button::setText( std::string s ) {
	text = s;
}

void Button::setPos( glm::vec2 p ) {
	pos = p;
}

glm::vec2 Button::getPos() const {
	return pos;
}

void Button::setSize( glm::vec2 s ) {
	size = s;
}

glm::vec2 Button::getSize() const {
	return size;
}

void Button::setVisible( GLboolean visible ) {
	if ( !visible ) {
		state = DISABLED;
	} else if ( !isVisible ) {
		state = RELEASED;
	}
	isVisible = visible;
	
}

void Button::setRotation( GLuint rot ) {
	GLuint r = rot % 90;
	if ( r != 0 ) {
		if ( r > 45 ) {
			rotation = ( rot / 90 ) * 90 + 90;
		} else {
			rotation = ( rot / 90 ) * 90;
		}
	} else {
		rotation = rot;
	}
}

void Button::setDraggable( GLboolean draggable ) {
	isDraggable = draggable;
}

void Button::render( Graphics& graphics ) {
	if ( isVisible ) {
		// render button texture
		if ( state == PRESSED ) {
			graphics.draw2DTexture( pressedTexture, pos, size, rotation );
		} else if ( state == RELEASED ) {
			graphics.draw2DTexture( releasedTexture, pos, size, rotation );
		} else {
			graphics.draw2DTexture( hoverTexture, pos, size, rotation );
		}

		// render button text
		graphics.renderText( fontType, std::string( text.c_str() ), pos + size / 2.0f, fontScale, textColor, HOR_CENTERED, VERT_CENTERED );
	}
}

void Button::setOnClickFunction( std::function<void()> fcn ) {
	onClickFunction = fcn;
}

void Button::setOnReleaseFunction( std::function<void()> fcn ) {
	onReleaseFunction = fcn;
}

void Button::setOnMouseMovementFunction( std::function<void()> fcn ) {
	onMouseMovementFunction = fcn;
}

GLboolean Button::isOverButton( glm::vec2 pos ) const {
	return ( pos.x >= this->pos.x && pos.x <= this->pos.x + size.x
		&& pos.y >= this->pos.y && pos.y <= this->pos.y + size.y );
}

