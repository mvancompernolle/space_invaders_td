#include "../includes/irrKlangAudio.h"



irrKlangAudio::irrKlangAudio() {
	soundEngine = irrklang::createIrrKlangDevice();
}


irrKlangAudio::~irrKlangAudio() {
	soundEngine->drop();
}

void irrKlangAudio::playSound( std::string sound, GLboolean loop ) {
	soundEngine->play2D( sound.c_str(), loop );
}

void irrKlangAudio::stopSound( std::string sound ) {

}

void irrKlangAudio::setMasterVolume( GLfloat volume ) {
	soundEngine->setSoundVolume( volume );
}

void irrKlangAudio::setAllSoundsPaused( GLboolean isPaused ) {
	soundEngine->setAllSoundsPaused( isPaused );
}

void irrKlangAudio::stopAllSounds() {
	soundEngine->stopAllSounds();
}
