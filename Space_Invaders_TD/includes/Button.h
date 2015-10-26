#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "OnClickObserver.h"
#include "Graphics.h"
#include <functional>
#include <string>
#include "TextChar.h"

enum ButtonState {
	PRESSED,
	RELEASED, 
	HOVER,
	DISABLED
};

class Button : public OnClickObserver {
public:
							Button( glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2( 10.0f ) );
							~Button();
	virtual void			onClick( glm::vec2 pos );
	virtual void			onRelease( glm::vec2 pos );
	virtual void			onMouseMovement( glm::vec2 pos );
	void					setTextures( Texture pressed, Texture released, Texture hover );
	void					setTextColor( glm::vec3 color );
	void					setText( std::string s );
	void					setPos( glm::vec2 p );
	glm::vec2				getPos() const;
	void					setSize( glm::vec2 s );
	glm::vec2				getSize() const;
	void					setVisible( GLboolean visible );
	void					setRotation( GLuint rot );
	void					setDraggable( GLboolean draggable );
	void					render( Graphics& graphics );
	void					setOnClickFunction( std::function<void()> fcn );
	void					setOnReleaseFunction( std::function<void()> fcn );
	void					setOnMouseMovementFunction( std::function<void()> fcn );
private:
	ButtonState				state;
	std::string				text;
	font					fontType;
	GLfloat					fontScale, rotation;
	glm::vec3				textColor;
	glm::vec2				pos, size;
	GLboolean				isVisible;
	Texture					releasedTexture, pressedTexture, hoverTexture;
	GLboolean				isDraggable;

	std::function<void()>	onClickFunction;
	std::function<void()>	onReleaseFunction;
	std::function<void()>	onMouseMovementFunction;

	GLboolean				isOverButton( glm::vec2 pos ) const;
};

#endif // BUTTON_H