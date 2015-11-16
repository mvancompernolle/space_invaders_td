#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include "OnKeyObserver.h"
#include "Input.h"
#include "SpawnInfo.h"
#include "TowerTypes.h"

enum SHAPE { RECTANGLE = 1, CIRCLE = RECTANGLE << 1, POLYGON = CIRCLE << 1};
enum COLLISION_TYPES { ENEMY = 1, BULLET = ENEMY << 1, TOWER = BULLET << 1, WALL = TOWER << 1, DESPAWN = WALL << 1};

enum COMPONENT_TYPE {
	NONE = 0, HEALTH = 1, WORLD = HEALTH << 1, RENDER = WORLD << 1,
	MOVEMENT = RENDER << 1, PATH = MOVEMENT << 1, SPAWN = PATH << 1,
	COLLISION = SPAWN << 1, PLAYER_INPUT = COLLISION << 1, DAMAGE = PLAYER_INPUT << 1,
	MONEY = DAMAGE << 1, SHOOT = MONEY << 1, FOLLOW = SHOOT << 1, DAMAGE_AURA = FOLLOW << 1,
	COMPONENT_SIZE = DAMAGE_AURA << 1, COMPONENT_NUM = 11
};

struct DamageComponent {
	float trueDmg, voidDmg, plasmaDmg, iceDmg;
	DamageComponent() : trueDmg( 1.0f ), voidDmg( 1.0f ), plasmaDmg( 1.0f ), iceDmg( 1.0f ) {}
	DamageComponent( float tDmg, float vDmg, float pDmg, float iDmg ) : trueDmg( tDmg ), voidDmg( vDmg ), plasmaDmg( pDmg ), iceDmg( iDmg ) {}
};

struct HealthComponent {
	float currHP, maxHP;
	float voidArmor, plasmaArmor, iceArmor;
	HealthComponent() : currHP( 0.0f ), maxHP( 0.0f ), voidArmor( 0.5f ), plasmaArmor( 0.5f ), iceArmor( 0.5f ) {}
	void takeDmg( DamageComponent dmg ) {
		currHP -= ( dmg.trueDmg + ( dmg.voidDmg * voidArmor ) +
			( dmg.plasmaDmg * plasmaArmor ) + ( dmg.iceDmg *iceArmor ) );
	}
};

struct WorldComponent {
	glm::vec2 pos, size;
	float rotation;
	WorldComponent() : pos( glm::vec2( 0, 0 ) ), size( glm::vec2( 10, 10 ) ) {}

	glm::vec2 getCenter() const {
		return pos + size * 0.5f;
	}
};

struct RenderComponent {
	glm::vec3 color;
	std::string textureName;
	RenderComponent() {}
};

struct MovementComponent {
	glm::vec2 vel;
	float defSpeed;
	MovementComponent() : vel( glm::vec2( 0.0f ) ), defSpeed( 0.0f ) {}
};

struct PathAIComponent {
	glm::vec2 target;
	unsigned pathIndex;
	PathAIComponent() : target( glm::vec2( 0.0f ) ), pathIndex( -1 ) {}
};

struct SpawnComponent {
	float dt;
	int round;
	unsigned numRounds, currSpawnNum;
	std::vector<SpawnInfo> spawnTypes;
	SpawnComponent() : dt( 0.0f ), round( 0 ), numRounds( 0 ), currSpawnNum( 0 ) {}
};

struct CollisionComponent {
	SHAPE shape;
	unsigned collisionID;
	unsigned collisionMask;
	float collisionScale;
	CollisionComponent() : shape( RECTANGLE ), collisionID(0), collisionMask(0), collisionScale( 1.0f ) {}
};

struct PlayerInputComponent {
	std::vector<unsigned> leftInputs;
	std::vector<unsigned> rightInputs;
	std::vector<unsigned> shootPrimaryInputs;
	std::vector<unsigned> shootSecondaryInputs;

	PlayerInputComponent() {
		leftInputs.push_back( (unsigned)KEY_A );
		rightInputs.push_back( (unsigned)KEY_D );
		leftInputs.push_back( (unsigned)KEY_LEFT );
		rightInputs.push_back( (unsigned)KEY_RIGHT );
		shootPrimaryInputs.push_back( (unsigned)MOUSE_BUTTON_LEFT );
		shootSecondaryInputs.push_back( (unsigned)MOUSE_BUTTON_RIGHT );
	}
};

struct MoneyComponent {
	int value;
	MoneyComponent() : value( 0 ) {}
};

struct ShootComponent {
	float attackSpeed, timePassed, range, bulletSpeed;
	int entTarget;
	DamageComponent bulletDmg;
	TOWER_TYPE towerType;
	std::string bulletTexture;
	ShootComponent() : attackSpeed( 1.0f ), timePassed( 0.0f ), range(800.0f), bulletSpeed(500.0f), entTarget( -1 ), bulletDmg(),
	bulletTexture(""), towerType(TOWER_NONE) {}
};

struct FollowComponent {
	int entTarget;
	float turnRate;
	FollowComponent() : entTarget( -1 ), turnRate( 180.0f ) {}
};

struct DmgAuraComponent {
	float timePassed, pulseSpeed, range;
	DamageComponent dmg;
	DmgAuraComponent() : pulseSpeed(0.1f), timePassed(0.1f), range(100.0f), dmg( 0.0f, 0.0f, 0.0f, 0.0f ){}
};

#endif // COMPONENT_H