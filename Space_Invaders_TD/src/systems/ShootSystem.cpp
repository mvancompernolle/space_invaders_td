#include "systems/ShootSystem.h"
#include "EntityFactory.h"
#include <algorithm>
#include <limits>
#include <ctime>

ShootSystem::ShootSystem( unsigned* enemiesLeft ) : System(enemiesLeft) {
	flags = ( WORLD | SHOOT );
	srand( time( NULL ) );
	systemName = "shootSystem";
}


ShootSystem::~ShootSystem() {
}

void ShootSystem::clear() {
	System::clear();
	registeredEntities.clear();
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
			spawnBullets( world, pos, dt );
		}
	}
}

void ShootSystem::spawnBullets( World* world,  unsigned pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	ShootComponent& shootComp = world->shootComponents[world->getComponentIndex( pos, SHOOT )];

	// create a base bullet
	Entity ent( WORLD | RENDER | MOVEMENT | COLLISION | DAMAGE | FOLLOW | PARENT );
	// init world data
	ent.world.size = worldComp.size * shootComp.bulletSize;
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
	// init parent data
	ent.parent.parentEnt = pos;

	// add other effects based on the shoot type
	switch ( shootComp.towerType ) {
	case TOWER_VOID_TRUE:
		// give bullets a damage aura
		ent.componentTypes |= DAMAGE_AURA;
		ent.dmgAura.dmg.trueDmg = 1.0f;
		ent.dmgAura.pulseSpeed = 0.1f;
		ent.dmgAura.range = 200.0f;
		break;
	case TOWER_TRUE_PLASMA: {
		// have the tower shoot 1 true and 2 plasma bullets
		// disable bullet lockon
		ent.componentTypes = ( ent.componentTypes & ~( FOLLOW ) );
		// calc vel so that it aims ahead
		glm::vec2 predictedLocation;
		MovementComponent& targetMovComp = world->movementComponents[world->getComponentIndex( shootComp.entTarget, MOVEMENT )];
		float relativeSpeed = std::abs( glm::length( targetMovComp.vel ) - glm::length( ent.movement.vel ) );
		float dist = glm::distance( worldComp.getCenter(), worldComp2.getCenter() );
		float time = 0.0f;
		if ( relativeSpeed != 0.0f ) {
			time = dist / relativeSpeed;
		}
		predictedLocation = worldComp2.getCenter() + targetMovComp.vel * time;
		// shoot bullet toward predicted location
		ent.movement.vel = glm::normalize( predictedLocation - worldComp.getCenter() ) * ent.movement.defSpeed;

		// shoot 2 more weaker plasma bullets
		Entity plasmaBullet = ent;
		plasmaBullet.world.size = worldComp.size * shootComp.bulletSize;
		plasmaBullet.damage.trueDmg = 0.0f;
		plasmaBullet.damage.plasmaDmg = ent.damage.trueDmg / 2.0f;
		for ( int i = 0; i < 2; ++i ) {
			plasmaBullet.movement.defSpeed = plasmaBullet.movement.defSpeed * ( 100.0f / ( 75 + rand() % 50 ) );
			// get angle of bullet
			float rotation = glm::degrees( std::atan2( ent.movement.vel.y, ent.movement.vel.x ) ) + ( -10 + rand() % 20 );
			plasmaBullet.movement.vel.x = std::cos( glm::radians( rotation ) ) * plasmaBullet.movement.defSpeed;
			plasmaBullet.movement.vel.y = std::sin( glm::radians( rotation ) ) * plasmaBullet.movement.defSpeed;
			plasmaBullet.render.textureName = "bullet_plasma";
			addAddition( plasmaBullet );
		}
	}
	break;
	case TOWER_VOID_PLASMA: {
		// make the bullet shoot bullets
		ent.componentTypes |= SHOOT;
		ent.shoot.attackSpeed = 0.4f;
		ent.shoot.bulletDmg.trueDmg = ent.shoot.bulletDmg.iceDmg = ent.shoot.bulletDmg.voidDmg = 0.0f;
		ent.shoot.bulletDmg.plasmaDmg = 10.0f;
		ent.shoot.timePassed = ent.shoot.attackSpeed;
		ent.shoot.range = 500.0f;
		ent.shoot.bulletSize = 0.8f;
		ent.shoot.bulletSpeed = 1000.0f;
		ent.shoot.bulletTexture = "bullet_plasma";
	}
	break;
	case TOWER_ICE_VOID: {
		// make the bullets have a slow aoe
		ent.componentTypes |= AOE;
		ent.aoe.dmg.voidDmg = shootComp.bulletDmg.iceDmg * 0.25f;
		ent.aoe.dmg.slowInfo.percentSpeed = shootComp.bulletDmg.slowInfo.percentSpeed * 0.75;
		ent.aoe.dmg.slowInfo.timeLeft = shootComp.bulletDmg.slowInfo.timeLeft;
		ent.aoe.dmg.slowInfo.type = shootComp.bulletDmg.slowInfo.type;
		ent.aoe.range = shootComp.range * 0.25f;
		ent.aoe.type = ( AOE_SLOW | AOE_DAMAGE );
	}
	break;
	}
	addAddition( ent );
}