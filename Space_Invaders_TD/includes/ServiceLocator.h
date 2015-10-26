#pragma once

#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include "Graphics.h"
#include "Audio.h"
#include "Input.h"

class ServiceLocator {
public:
	ServiceLocator();
	~ServiceLocator();
	static void provideAudio( Audio* service );
	static void provideGraphics( Graphics* service );
	static void provideInput( Input* service );
	static Graphics& getGraphics();
	static Audio& getAudio();
	static Input& getInput();
private:
	static Graphics		*graphicsService;
	static Audio		*audioService;
	static Input		*inputService;
};

#endif // SERVICE_LCOATOR_H

