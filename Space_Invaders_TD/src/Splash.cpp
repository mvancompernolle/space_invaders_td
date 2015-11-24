#include "../includes/Splash.h"
#include "../includes/ServiceLocator.h"
#include "../includes/ResourceManager.h"


Splash::Splash()
	: dt(0.0f), currentTime(0.0f), charCooldown(0.0f), title("Space Invaders TD"), gameBy("By: Matthew VanCompernolle"),
	numTitleChars(0), numByChars(0), width(GAME_WIDTH), height(GAME_HEIGHT)
{
}


Splash::~Splash()
{
}

STATE Splash::update( GLfloat dt ) {
	currentTime += dt;
	charCooldown += dt;

	if ( charCooldown > animationTime ) {
		charCooldown -= animationTime;
		if ( numTitleChars < title.size() ) {
			numTitleChars++;
			if ( numTitleChars % 5 == 0 ) {
				ServiceLocator::getAudio().playSound( ResourceManager::getPath( "sounds" ) + "splash_title.wav", GL_FALSE );
			}
		} else if ( numByChars < gameBy.size() ) {
			numByChars++;
			if ( numByChars == gameBy.size() ) {
				ServiceLocator::getAudio().playSound( ResourceManager::getPath( "sounds" ) + "splash_title.wav", GL_FALSE );
			}
		}
	}

	return ( (currentTime < splashTime) ? SPLASH : MAIN_INIT);
}

void Splash::render() {
	// render title
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), title.substr( 0, numTitleChars ).c_str(),
		glm::vec2( width * 0.5f, height * 0.5f ), 3.0f, glm::vec3( 0.0f, 0.0f, 1.0f ), HOR_CENTERED, VERT_CENTERED );

	// render who game is by
	ServiceLocator::getGraphics().renderText( ResourceManager::getFont( "default" ), gameBy.substr( 0, numByChars ).c_str(),
		glm::vec2( width * 0.5f, height * 0.6f ), 1.0f, glm::vec3( 0.2f, 0.4f, 0.8f ), HOR_CENTERED, VERT_CENTERED );
}