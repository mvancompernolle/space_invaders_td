#include "systems/ShootSystem.h"
#include <algorithm>
#include <limits>

ShootSystem::ShootSystem() {
	flags = ( WORLD | SHOOT );
}


ShootSystem::~ShootSystem() {
}

void ShootSystem::registerEntity( unsigned entity ) {
	registeredEntities.push_back( entity );
}

void ShootSystem::unregisterEntity( unsigned entity ) {
	registeredEntities.erase( std::remove( registeredEntities.begin(), registeredEntities.end(), entity ), registeredEntities.end() );
}

void ShootSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	ShootComponent& shootComp = world->shootComponents[world->getComponentIndex( pos, SHOOT )];

	// calculate target to shoot at
	if ( shootComp.entTarget == -1 ||
		glm::distance( worldComp.getCenter(), world->worldComponents[world->getComponentIndex( shootComp.entTarget, WORLD )].getCenter() ) ) {
		int targetEnt = -1;
		float minDist = std::numeric_limits<float>::max();
		for ( int ent : registeredEntities ) {
			WorldComponent& worldComp2 = world->worldComponents[world->getComponentIndex( ent, WORLD )];
			float dist = glm::distance( worldComp.getCenter(), worldComp2.getCenter() );
			if ( dist <= shootComp.range && dist < minDist ) {
				minDist = dist;
				targetEnt = ent;
			}
		}
		// set target
		shootComp.entTarget = targetEnt;
	}

	// shoot bullet at target
	if ( shootComp.entTarget != -1 ) {
		shootComp.timePassed += dt;
		if ( shootComp.timePassed >= shootComp.attackSpeed ) {
			shootComp.timePassed -= shootComp.attackSpeed;
			spawnBullets( world, pos );
		}
	}
}

void ShootSystem::spawnBullets( World* world,  unsigned pos ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	ShootComponent& shootComp = world->shootComponents[world->getComponentIndex( pos, SHOOT )];

	// create a base bullet
	Entity ent( WORLD | RENDER | MOVEMENT | COLLISION | DAMAGE | FOLLOW );
	// init world data
	ent.world.size = worldComp.size * 0.25f;
	ent.world.pos = worldComp.getCenter() - ( ent.world.size * 0.5f );
	ent.world.rotation = 0.0f;
	// init render data
	ent.render.textureName = shootComp.bulletTexture;
	// init movement data
	WorldComponent& worldComp2 = world->worldComponents[world->getComponentIndex( shootComp.entTarget, WORLD )];
	ent.movement.defSpeed = shootComp.bulletSpeed;
	ent.movement.vel = glm::normalize( worldComp2.getCenter() - worldComp.getCenter() ) * ent.movement.defSpeed;
	// init collision data
	ent.collision.collisionID = BULLET;
	ent.collision.shape = CIRCLE;
	ent.collision.collisionMask = ENEMY;
	// init damage data
	ent.damage = shootComp.bulletDmg;
	// init follow data
	ent.follow.entTarget = shootComp.entTarget;

	// add other effects based on the shoot type
	switch ( shootComp.towerType ) {
	case TOWER_VOID_TRUE:
		// give bullets a damage aura
		ent.componentTypes |= DAMAGE_AURA;
		ent.dmgAura.dmg.trueDmg = 1.0f;
		ent.dmgAura.pulseSpeed = 0.1f;
		ent.dmgAura.range = 200.0f;
		break;
	}

	additions.push_back( ent );
}