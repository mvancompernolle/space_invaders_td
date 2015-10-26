#pragma once

#ifndef IRRKLANG_AUDIO_H
#define IRRKLANG_AUDIO_H

#include "Audio.h"
#include <irrklang/irrKlang.h>

class irrKlangAudio : public Audio {
public:
	irrKlangAudio();
	~irrKlangAudio();
	virtual void playSound( std::string sound, GLboolean loop = GL_FALSE );
	virtual void stopSound( std::string sound );
	virtual void setMasterVolume( GLfloat volume );
	virtual void setAllSoundsPaused( GLboolean isPaused );
	virtual void stopAllSounds();

private:
	irrklang::ISoundEngine*	soundEngine;
};

#endif // IRRKLANG_AUDIO_H