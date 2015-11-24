#include "../includes/MainMenu.h"
#include "../includes/ServiceLocator.h"
#include "../includes/ResourceManager.h"

MainMenu::MainMenu() {
	width = GAME_WIDTH;
	height = GAME_HEIGHT;

	// initialize buttons
	bExit = Button( glm::vec2( width * 0.1f, height * 0.7f ), glm::vec2( width * 0.2f, height * 0.2f ) );
	bRules = Button( glm::vec2( width * 0.4f, height * 0.7f ), glm::vec2( width * 0.2f, height * 0.2f ) );
	bPlay = Button( glm::vec2( width * 0.7f, height * 0.7f ), glm::vec2( width * 0.2f, height * 0.2f ) );
	bExit.setText( "Exit Game" );
	bRules.setText( "Rules" );
	bPlay.setText( "Play Game" );
	bExit.setOnClickFunction( [&]() {
		state = EXIT;
	} );
	bRules.setOnClickFunction( [&] () {
		state = RULES_INIT;
	} );
	bPlay.setOnClickFunction( [&]() {
		state = PLAY;
	} );

	// load background
	ResourceManager::loadTexture( "main_menu_background.jpg", GL_FALSE, "main_menu_background" );

	// play eerie music
	ServiceLocator::getAudio().setMasterVolume( 0.5f );
	ServiceLocator::getAudio().playSound( ResourceManager::getPath( "sounds" ) + "theme.mp3", GL_TRUE );
}


MainMenu::~MainMenu() {
}

void MainMenu::init() {
	state = MAIN;
	ServiceLocator::getInput().addOnClickObserver( &bPlay );
	ServiceLocator::getInput().addOnClickObserver( &bExit );
	ServiceLocator::getInput().addOnClickObserver( &bRules );
}

STATE MainMenu::getMenuState() {
	if ( state != MAIN ) {
		// Unregister from input
		ServiceLocator::getInput().removeOnClickObserver( &bExit );
		ServiceLocator::getInput().removeOnClickObserver( &bPlay );
		ServiceLocator::getInput().removeOnClickObserver( &bRules );
	}
	return state;
}

void MainMenu::render( GLfloat dt ) {
	// render background
	ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "main_menu_background" ), glm::vec2( 0.0f ), glm::vec2( width, height ), 0.0f );

	// render buttons
	bPlay.render( ServiceLocator::getGraphics() );
	bRules.render( ServiceLocator::getGraphics() );
	bExit.render( ServiceLocator::getGraphics() );

	// render game title
	ServiceLocator::getGraphics().renderText(ResourceManager::getFont("default"), "Space Invaders TD", glm::vec2( width / 2.0f, height * 0.43f ), 
		2.5f, glm::vec3(0.0f, 0.75f, 0.0f), HOR_CENTERED, VERT_CENTERED);
}
