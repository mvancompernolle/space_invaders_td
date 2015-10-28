#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Entity;

struct Component {

};

enum SHAPE { RECTANGLE, CIRCLE };
enum COMPONENT_TYPE {
	HEALTH = 1, WORLD = HEALTH << 1, RENDER = WORLD << 1,
	MOVEMENT = RENDER << 1, PATH = MOVEMENT << 1, SPAWN = PATH << 1, COLLISION = PATH << 1
};

struct HealthComponent : Component {
	const static COMPONENT_TYPE type = HEALTH;
	float currHP, maxHP;
	HealthComponent() : currHP(0.0f), maxHP(0.0f) {}
};

struct WorldComponent : Component {
	const static COMPONENT_TYPE type = WORLD;
	glm::vec2 pos, size;
	float rotation;
	WorldComponent() : pos( glm::vec2( 0, 0 ) ), size( glm::vec2( 10, 10 ) ) {}
};

struct RenderComponent : Component {
	const static COMPONENT_TYPE type = RENDER;
	glm::vec3 color;
	std::string textureName;
	RenderComponent() {}
};

struct MovementComponent : Component {
	const static COMPONENT_TYPE type = MOVEMENT;
	glm::vec2 vel;
	float defSpeed;
	MovementComponent() : vel( glm::vec2(0.0f) ), defSpeed(0.0f) {}
};

struct PathAIComponent : Component {
	const static COMPONENT_TYPE type = PATH;
	glm::vec2 target;
	unsigned pathIndex;
	PathAIComponent() : target( glm::vec2( 0.0f ) ), pathIndex(0) {}
};

struct SpawnInfo {
	unsigned num;
	Entity* spawnType;
};

struct SpawnComponent : Component {
	const static COMPONENT_TYPE type = SPAWN;
	float spawnRate, dt;
	unsigned round, numRounds, currSpawnNum;
	std::vector<SpawnInfo> spawnTypes;
	SpawnComponent() : spawnRate(1.0f), dt(0.0f), round(0), numRounds(0), currSpawnNum(0) {}
};

struct CollisionComponent : Component {
	const static COMPONENT_TYPE type = COLLISION;
	SHAPE shape;
	CollisionComponent() : shape( RECTANGLE ) {}
};

#endif // COMPONENT_H