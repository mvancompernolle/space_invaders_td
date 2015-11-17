#include "systems/DamageAuraSystem.h"
#include <algorithm>


DamageAuraSystem::DamageAuraSystem( unsigned* enemiesLeft ) : System(enemiesLeft){
	flags = WORLD | DAMAGE_AURA;
}


DamageAuraSystem::~DamageAuraSystem() {
}

void DamageAuraSystem::registerEntity( unsigned entity ) {
	registeredEntities.push_back( entity );
}

void DamageAuraSystem::unregisterEntity( unsigned entity ) {
	registeredEntities.erase( std::remove( registeredEntities.begin(), registeredEntities.end(), entity ), registeredEntities.end() );
}

void DamageAuraSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	DmgAuraComponent& dmgAuraComp = world->dmgAuraComponents[world->getComponentIndex( pos, DAMAGE_AURA )];

	dmgAuraComp.timePassed += dt;
	if ( dmgAuraComp.timePassed >= dmgAuraComp.pulseSpeed ) {
		dmgAuraComp.timePassed -= dmgAuraComp.pulseSpeed;

		// loop over enemies and apply damage aura
		for ( unsigned ent : registeredEntities ) {
			WorldComponent& worldComp2 = world->worldComponents[world->getComponentIndex( ent, WORLD )];
			if ( glm::distance( worldComp.getCenter(), worldComp2.getCenter() ) < dmgAuraComp.range ) {
				HealthComponent& healthComp = world->healthComponents[world->getComponentIndex( ent, HEALTH )];
				healthComp.takeDmg( dmgAuraComp.dmg );
			}
		}
	}
}
