#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include "GLFWGraphics.h"
#include "GLFWInput.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "irrKlangAudio.h"
#include "consts.h"
#include "SpaceInvadersTD.h"

// input callbacks
void keyCallBack( GLFWwindow* window, GLint key, GLint scanCode, GLint action, GLint mode );
void cursorPosCallBack( GLFWwindow* window, GLdouble xPos, GLdouble yPos );
void mouseButtonCallBack( GLFWwindow* window, GLint button, GLint action, GLint mods );
void scrollCallBack( GLFWwindow* window, GLdouble xOffset, GLdouble yOffset );

int main() {
	// set resource paths
	ResourceManager::setShaderPath( "resources/shaders/" );
	ResourceManager::setTexturePath( "resources/textures/" );
	ResourceManager::setPath( "fonts", "resources/fonts/" );
	ResourceManager::setPath( "sounds", "resources/sounds/" );
	ResourceManager::setPath( "text", "resources/text/" );

	// initialize services
	Input* input = new GLFWInput();
	GLFWGraphics* gr = new GLFWGraphics("Space Invaders TD" );
	Graphics *graphics = gr;
	Audio* audio = new irrKlangAudio();

	ServiceLocator::provideGraphics( graphics );
	ServiceLocator::provideInput( input );
	ServiceLocator::provideAudio( audio );

	// load default font
	ResourceManager::loadFont( "FreeSans.ttf", "default", 36.0f );

	// initialize mouse and keyboard
	glfwSetInputMode( gr->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	glfwSetCursorPosCallback( gr->getWindow(), cursorPosCallBack );
	glfwSetMouseButtonCallback( gr->getWindow(), mouseButtonCallBack );
	glfwSetScrollCallback( gr->getWindow(), scrollCallBack );
	glfwSetKeyCallback( gr->getWindow(), keyCallBack );

	// create game
	STATE current = PLAY;
	SpaceInvadersTD game;

	// start the game loop
	GLfloat dt = 0.0f, lastTime = 0.0f;
	GLboolean running = GL_TRUE;
	while ( graphics->windowIsValid() && running ) {
		// calculate dt
		GLfloat currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		glfwPollEvents();

		// keep dt small
		if ( dt > 0.0333f ) {
			dt = 0.0333f;
		}

		// render the game
		glClear( GL_COLOR_BUFFER_BIT );

		switch ( current ) {
		case SPLASH:
			break;

		case MAIN_INIT:
		case MAIN:
			break;

		case RULES_INIT:
		case RULES:
			break;

		case PLAY:
			game.init();
		case GAME:
			current = game.update( dt );
			game.render();
			break;

		case EXIT:
			// show simple credits then exit
			ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), "Thank you for playing!", glm::vec2( gr->getDimensions().x, gr->getDimensions().y ) / 2.0f,
				3.0f, glm::vec3( 1.0f ), HOR_CENTERED, VERT_CENTERED );
			graphics->swapBuffers();
			Sleep(3000);
			running = GL_FALSE;
			break;
		}

		graphics->swapBuffers();
	}

	//delete graphics;
	delete input;
	delete graphics;
	delete audio;

	return 0;
}

void keyCallBack( GLFWwindow* window, GLint key, GLint scanCode, GLint action, GLint mode ) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
	if ( key >= 0 && key < 1024 ) {
		if ( action == GLFW_PRESS ) {
			ServiceLocator::getInput().setKeyPressed( key );
		} else if ( action == GLFW_RELEASE ) {
			ServiceLocator::getInput().setKeyReleased( key );
		}
	}
}

void cursorPosCallBack( GLFWwindow* window, GLdouble xPos, GLdouble yPos ) {
	ServiceLocator::getInput().setMousePos( xPos, yPos );
}

void mouseButtonCallBack( GLFWwindow* window, GLint button, GLint action, GLint mods ) {
	if ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1 ) {
		ServiceLocator::getInput().setMouseButtonClicked( button );
	} else if ( action == GLFW_PRESS ) {
		ServiceLocator::getInput().setKeyPressed( button );
	} else {
		ServiceLocator::getInput().setKeyReleased( button );
	}
}

void scrollCallBack( GLFWwindow* window, GLdouble xOffset, GLdouble yOffset ) {
	ServiceLocator::getInput().scrollOffset( xOffset, yOffset );
}