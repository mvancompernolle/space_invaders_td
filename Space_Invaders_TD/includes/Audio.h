#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <GL/glew.h>

class Audio {
public:
	Audio();
	virtual ~Audio();
	virtual void playSound( std::string sound, GLboolean loop = GL_FALSE ) = 0;
	virtual void stopSound( std::string sound ) = 0;
	virtual void setMasterVolume( GLfloat volume ) = 0;
	virtual void setAllSoundsPaused( GLboolean isPaused ) = 0;
	virtual void stopAllSounds() = 0;
};

#endif // AUDIO_H