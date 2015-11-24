#pragma once

#ifndef DAMAGE_AURA_SYSTEM
#define DAMAGE_AURA_SYSTEM

#include "systems/system.h"

class DamageAuraSystem : public System {
public:
	DamageAuraSystem( unsigned* enemiesLeft );
	~DamageAuraSystem();
	void clear();
	void registerEntity( unsigned entity );
	void unregisterEntity( unsigned entity );
	void update( World* world, int pos, float dt );
private:
	std::vector<unsigned> registeredEntities;
};

#endif // DAMAGE_AURA_SYSTEM

