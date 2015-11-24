#include "../includes/Rules.h"
#include "../includes/ResourceManager.h"
#include "../includes/ServiceLocator.h"
#include <fstream>
#include <iostream>


Rules::Rules()
	: tbRules(glm::vec2(GAME_WIDTH * 0.3f, GAME_HEIGHT * 0.05f), glm::vec2(GAME_WIDTH * 0.65f, GAME_HEIGHT * 0.9f), 8, ResourceManager::getFont("default") )
{
	width = GAME_WIDTH;
	height = GAME_HEIGHT;

	// initialize buttons
	bBack = Button( glm::vec2( GAME_WIDTH * 0.05f, GAME_HEIGHT * 0.05f ), glm::vec2( GAME_WIDTH * 0.2f, GAME_HEIGHT * 0.1f ) );
	bBack.setText( "Back" );
	bBack.setOnClickFunction( [&] () {
		current = MAIN_INIT;
	} );

	// load background
	ResourceManager::loadTexture( "rules_background.jpg", GL_FALSE, "rules_background" );

	// load game rules from file
	tbRules.setPadding( 24, 24 );
	std::fstream rulesFile( ResourceManager::getPath( "text" ) + "rules.txt", std::ios_base::in );
	if ( rulesFile.is_open() ) {
		char tmp[1024];
		while ( rulesFile.good() ) {
			if ( rulesFile.peek() == '\n' ) {
				tbRules.addNewLine();
			}
			rulesFile.getline( tmp, 1024 );
			tbRules.addText( tmp, GL_TRUE );
		}

		rulesFile.close();
	} else {
		std::cout << "Failed to load rules file" << std::endl;
	}
}

Rules::~Rules()
{
}

void Rules::init() {
	current = RULES;
	tbRules.scrollToLine( 0 );
	ServiceLocator::getInput().addOnClickObserver( &bBack );
	ServiceLocator::getInput().addOnClickObserver( &tbRules );
	ServiceLocator::getInput().addOnScrollObserver( &tbRules );
}

STATE Rules::update() {
	if ( current != RULES ) {
		// unregister input listeners
		ServiceLocator::getInput().removeOnClickObserver( &bBack );
		ServiceLocator::getInput().removeOnClickObserver( &tbRules );
		ServiceLocator::getInput().removeOnScrollObserver( &tbRules );
	}
	return current;
}

void Rules::render() {
	ServiceLocator::getGraphics().draw2DTexture( ResourceManager::getTexture( "rules_background" ), glm::vec2( 0.0f ), glm::vec2( width, height ), 0.0f );
	tbRules.render( ServiceLocator::getGraphics() );
	bBack.render( ServiceLocator::getGraphics() );
}
