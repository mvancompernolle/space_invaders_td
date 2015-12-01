#include "../includes/Textbox.h"
#include "../includes/Graphics.h"
#include "../includes/ResourceManager.h"

Textbox::Textbox( glm::vec2 pos, glm::vec2 size, GLuint borderSize, const font& fontType, GLfloat fontScale, bool scrollable ) :
	pos( pos ), size( size ), borderSize( borderSize ), fontType( fontType ), fontScale( fontScale ), borderColor( glm::vec3( 0.0f ) ),
	backgroundColor( glm::vec3( 1.0f ) ), textColor( glm::vec3( 0.0f ) ), paddingHorizontal( 8.0f ), paddingVertical( 8.0f ), currHorizontalOffset( 0.0f ),
	currentLineNumber( 0 ), lineSpacing( 1.5f ), firstLineInView( 0 ), btnSize( 32.0f ), currentMousePos( glm::vec2( -1 ) ), mouseClickOffset( 0.0f ),
	scrollable( scrollable ) {

	// create scroll bar
	if ( scrollable ) {
		// set scroll button textures
		Texture pressed = ResourceManager::loadTexture( "scroll_btn_pressed.png", GL_TRUE, "scroll_btn_pressed_default" );
		Texture released = ResourceManager::loadTexture( "scroll_btn_released.png", GL_TRUE, "scroll_btn_released_default" );
		Texture hover = ResourceManager::loadTexture( "scroll_btn_hover.png", GL_TRUE, "scroll_btn_hover_default" );
		bScrollUp.setTextures( pressed, released, hover );
		bScrollUp.setSize( btnSize );
		bScrollUp.setPos( glm::vec2( pos.x + size.x - borderSize - btnSize.x, pos.y + borderSize ) );
		bScrollUp.setOnClickFunction( [&]() {
			if ( firstLineInView > 0 ) {
				firstLineInView--;
				updateScrollBar();
			}
		} );

		bScrollDown.setTextures( pressed, released, hover );
		bScrollDown.setSize( btnSize );
		bScrollDown.setPos( glm::vec2( pos.x + size.x - borderSize - btnSize.x, pos.y + size.y - borderSize - btnSize.y ) );
		bScrollDown.setRotation( 180 );
		bScrollDown.setOnClickFunction( [&]() {
			if ( firstLineInView < currentLineNumber + 1 - getNumLinesThatFit() ) {
				firstLineInView++;
				updateScrollBar();
			}
		} );

		Texture scrollbarPressed = ResourceManager::loadTexture( "scroll_bar_pressed.png", GL_TRUE, "scroll_bar_pressed_default" );
		Texture scrollbarReleased = ResourceManager::loadTexture( "scroll_bar_released.png", GL_TRUE, "scroll_bar_released_default" );
		Texture scrollbarHover = ResourceManager::loadTexture( "scroll_bar_hover.png", GL_TRUE, "scroll_bar_hover_default" );
		bScrollBar.setTextures( scrollbarPressed, scrollbarReleased, scrollbarHover );
		bScrollBar.setDraggable( GL_TRUE );
		updateScrollBar();
		bScrollBar.setOnMouseMovementFunction( [&]() {
			dragScrollBar();
		} );
	} else {
		btnSize = glm::vec2(0.0f);
	}
}

Textbox::~Textbox() {
}

void Textbox::addText( std::string newText, GLboolean newLine ) {
	if ( newText.size() == 0 )
		return;

	appendText( newText, textColor );

	if ( newLine ) {
		addNewLine();
	}
}

void Textbox::addText( std::string newText, glm::vec3 color, GLboolean newLine ) {
	if ( newText.size() == 0 )
		return;

	appendText( newText, color );

	if ( newLine ) {
		addNewLine();
	}
}

void Textbox::appendText( std::string newText, glm::vec3 color ) {

	// get edges
	GLuint leftEdge = pos.x + paddingHorizontal + borderSize;
	GLuint rightEdge = pos.x + size.x - paddingHorizontal - borderSize - btnSize.x;

	// loop through each token of text
	std::string delimiter = " ";
	GLfloat delimOffset = getStringWidth( " " );
	std::string token;
	GLuint start = 0;
	GLuint end = newText.find( delimiter );
	while ( end != std::string::npos ) {
		// get the next token
		token = newText.substr( start, end - start );

		// calculate the width of the token
		GLfloat width = getStringWidth( token );

		// check to see if token fits on the same line
		if ( leftEdge + currHorizontalOffset + width > rightEdge ) {
			addNewLine();
		}
		tokens.push_back( StringToken( token, currHorizontalOffset, currentLineNumber, color ) );

		// move horizontal offset
		currHorizontalOffset += width + delimOffset;

		start = end + delimiter.length();
		end = newText.find( delimiter, start );
	}

	token = newText.substr( start, newText.size() );
	GLfloat width = getStringWidth( token );
	if ( leftEdge + currHorizontalOffset + width > rightEdge ) {
		addNewLine();
	}
	tokens.push_back( StringToken( token, currHorizontalOffset, currentLineNumber, color ) );
	currHorizontalOffset += width + delimOffset;
}

void Textbox::addNewLine() {
	currentLineNumber++;
	currHorizontalOffset = 0.0f;
	tokens.push_back( StringToken( "", currHorizontalOffset, currentLineNumber, textColor ) );
	// any time a new line is added, go to bottom of textbox
	firstLineInView = 0;
	if ( currentLineNumber >= getNumLinesThatFit() ) {
		firstLineInView = currentLineNumber + 1 - getNumLinesThatFit();
	}

	updateScrollBar();
}

void Textbox::clear() {
	tokens.clear();
	firstLineInView = 0;
	currentLineNumber = 0;
	currHorizontalOffset = 0;
}

void Textbox::setPadding( GLfloat horizontal, GLfloat vertical ) {
	paddingHorizontal = horizontal;
	paddingVertical = vertical;
}

void Textbox::setLineSpacing( GLfloat spacing ) {
	lineSpacing = spacing;
}

void Textbox::setTextColor( glm::vec3 color ) {
	textColor = color;
}

void Textbox::setBorderColor( glm::vec3 color ) {
	borderColor = color;
}

void Textbox::setBackgroundColor( glm::vec3 color ) {
	backgroundColor = color;
}

void Textbox::scrollToLine( GLuint lineNum ) {
	if ( lineNum < 0 || lineNum < currentLineNumber + 1 - getNumLinesThatFit() ) {
		firstLineInView = lineNum;
		updateScrollBar();
	}
}

void Textbox::render( Graphics& graphics ) {
	// render border rectangle
	graphics.draw2DBox( pos, size, borderColor );
	// render background rectangle 
	graphics.draw2DBox( pos + (GLfloat)borderSize, size - borderSize * 2.0f, backgroundColor );

	if ( tokens.size() == 0 )
		return;

	// get range of tokens to render based on which are in view
	int start = getFirstTokenOnLine( firstLineInView );
	int end = ( firstLineInView + getNumLinesThatFit() - 1 ) < currentLineNumber ? getFirstTokenOnLine( firstLineInView + getNumLinesThatFit() ) : tokens.size();
	// render visible lines of text
	for ( int i = start; i < end; ++i ) {
		StringToken& token = tokens[i];
		GLfloat yPos = pos.y + paddingVertical + borderSize + fontType['H'].bearing.y * ( ( token.lineNum - firstLineInView ) * lineSpacing * fontScale );
		graphics.renderText( fontType, token.str.c_str(),
			glm::vec2( pos.x + paddingHorizontal + borderSize + token.xPos, yPos ), fontScale, token.color );
	}

	// render scroll buttons
	if ( scrollable ) {
		bScrollUp.render( graphics );
		bScrollDown.render( graphics );
		bScrollBar.render( graphics );
	}
}

GLfloat	Textbox::getStringWidth( const std::string& str ) const {
	GLfloat width = 0;
	for ( char c : str ) {
		width += ( fontType.at( c ).advance >> 6 ) * fontScale;
	}
	return width;
}

GLint Textbox::getNumLinesThatFit() const {
	GLuint num = ( ( size.y - ( paddingVertical + borderSize ) * 2.0f ) ) / ( lineSpacing * fontType.at( 'H' ).bearing.y * fontScale );
	return num;
}

GLuint Textbox::getFirstTokenOnLine( GLuint line ) {
	if ( line < 0 || line > currentLineNumber || tokens.size() == 0 ) {
		return -1;
	}

	// implement inefficiently for now
	for ( int i = 0; i < tokens.size(); ++i ) {
		if ( tokens[i].lineNum == line ) {
			return i;
		}
	}
	return -1;
}

void Textbox::onClick( glm::vec2 pos ) {
	mouseClickOffset = pos.y - bScrollBar.getPos().y;
	if ( scrollable ) {
		bScrollUp.onClick( pos );
		bScrollDown.onClick( pos );
		bScrollBar.onClick( pos );
	}
}

void Textbox::onRelease( glm::vec2 pos ) {
	mouseClickOffset = 0;
	if ( scrollable ) {
		bScrollUp.onRelease( pos );
		bScrollDown.onRelease( pos );
		bScrollBar.onRelease( pos );
	}
}

void Textbox::onMouseMovement( glm::vec2 pos ) {
	currentMousePos = pos;
	if ( scrollable ) {
		bScrollUp.onMouseMovement( pos );
		bScrollDown.onMouseMovement( pos );
		bScrollBar.onMouseMovement( pos );
	}
}

void Textbox::onHorizontalScroll( GLfloat offset, glm::vec2 pos ) {

}

void Textbox::onVerticalScroll( GLfloat offset, glm::vec2 pos ) {
	if ( scrollable && isOverView( pos ) ) {
		if ( offset < 0 ) {
			if ( firstLineInView < currentLineNumber + 1 - getNumLinesThatFit() ) {
				firstLineInView++;
			}
		} else {
			if ( firstLineInView > 0 ) {
				firstLineInView--;
			}
		}
		updateScrollBar();
	}
}

void Textbox::updateScrollBar() {
	if ( scrollable ) {
		// calculate size
		glm::vec2 scrollBarSize( btnSize );
		GLfloat totalYSize = size.y - ( borderSize + btnSize.y ) * 2.0f;
		GLfloat sizeRatio = getNumLinesThatFit() / ( currentLineNumber + 1.0f );
		// if not enough text, make scrollbar max size
		if ( sizeRatio > 1.0f ) {
			sizeRatio = 1.0f;
		}
		scrollBarSize.y = sizeRatio * totalYSize;
		// set to min size if too small
		if ( scrollBarSize.y < btnSize.y / 2.0f ) {
			scrollBarSize.y = btnSize.y / 2.0f;
		}
		bScrollBar.setSize( scrollBarSize );

		// calculate pos
		glm::vec2 scrollBarPos( pos.x + size.x - ( borderSize + btnSize.x ), pos.y + borderSize + btnSize.y );
		// get where starting line is with respect to total number of lines
		GLfloat posRatio = firstLineInView / ( currentLineNumber + 1.0f );
		scrollBarPos.y += totalYSize * posRatio;
		if ( scrollBarSize.y == btnSize.y / 2.0f && scrollBarPos.y >= pos.y + size.y - borderSize - btnSize.y * 1.5f ) {
			scrollBarPos.y = pos.y + size.y - borderSize - btnSize.y * 1.5f;
		}
		bScrollBar.setPos( scrollBarPos );
	}
}

void Textbox::dragScrollBar() {
	if ( scrollable && currentLineNumber + 1 > getNumLinesThatFit() ) {
		GLfloat yOffset = currentMousePos.y - ( mouseClickOffset + bScrollBar.getPos().y );
		GLfloat totalYSize = size.y - ( borderSize + btnSize.y ) * 2.0f;
		GLfloat lineRatio = yOffset / totalYSize;
		firstLineInView = lineRatio * currentLineNumber + firstLineInView;
		if ( firstLineInView < 0 ) {
			firstLineInView = 0;
		} else if ( firstLineInView > currentLineNumber + 1 - getNumLinesThatFit() ) {
			firstLineInView = currentLineNumber + 1 - getNumLinesThatFit();
		}
		updateScrollBar();
	}
}

GLboolean Textbox::isOverView( glm::vec2 pos ) const {
	return ( pos.x >= this->pos.x && pos.x <= this->pos.x + size.x
		&& pos.y >= this->pos.y && pos.y <= this->pos.y + size.y );
}

std::fstream &operator<<( std::fstream &fs, const Textbox& t ) {
	for ( int i = 0; i < t.tokens.size(); ++i ) {
		if ( t.tokens[i].lineNum != 0 && i != 0 && t.tokens[i].lineNum != t.tokens[i - 1].lineNum ) {
			fs << std::endl;
		} else {
			fs << t.tokens[i].str << " ";
		}

	}
	return fs;
}