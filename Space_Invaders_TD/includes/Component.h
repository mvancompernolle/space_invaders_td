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

enum SHAPE { RECTANGLE = 1, CIRCLE = RECTANGLE << 1, POLYGON = CIRCLE << 1};
enum COLLISION_TYPES { ENEMY = 1, BULLET = ENEMY << 1, TOWER = BULLET << 1, WALL = TOWER << 1, DESPAWN = WALL << 1};

enum COMPONENT_TYPE {
	NONE = 0, HEALTH = 1, WORLD = HEALTH << 1, RENDER = WORLD << 1,
	MOVEMENT = RENDER << 1, PATH = MOVEMENT << 1, SPAWN = PATH << 1,
	COLLISION = SPAWN << 1, PLAYER_INPUT = COLLISION << 1, DAMAGE = PLAYER_INPUT << 1,
	MONEY = DAMAGE << 1,
	COMPONENT_SIZE = MONEY << 1,
};

struct HealthComponent {
	float currHP, maxHP;
	float voidArmor, plasmaArmor, iceArmor;
	HealthComponent() : currHP( 0.0f ), maxHP( 0.0f ), voidArmor( 0.5f ), plasmaArmor( 0.5f ), iceArmor( 0.5f ) {}
};

struct DamageComponent {
	float trueDmg, voidDmg, plasmaDmg, iceDmg;
	DamageComponent() : trueDmg( 1.0f ), voidDmg( 1.0f ), plasmaDmg( 1.0f ), iceDmg( 1.0f ) {}
};

struct WorldComponent {
	glm::vec2 pos, size;
	float rotation;
	WorldComponent() : pos( glm::vec2( 0, 0 ) ), size( glm::vec2( 10, 10 ) ) {}
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
		shootPrimaryInputs.push_back( (unsigned)MOUSE_BUTTON_LEFT );
		shootSecondaryInputs.push_back( (unsigned)MOUSE_BUTTON_RIGHT );
	}
};

struct MoneyComponent {
	int value;
	MoneyComponent() : value( 0 ) {}
};

#endif // COMPONENT_H