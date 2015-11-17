#pragma once

#ifndef PLAYER_INPUT_SYSTEM_H
#define PLAYER_INPUT_SYSTEM_H

#include "System.h"

class PlayerInputSystem : public System {
public:
	PlayerInputSystem( unsigned* enemiesLeft );
	~PlayerInputSystem();
	void update( World* world, int pos, float dt );
};

#endif // PLAYER_INPUT_SYSTEM_H