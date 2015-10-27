#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Component {

};

enum SHAPE { RECTANGLE, CIRCLE };
enum COMPONENT_TYPE {
	HEALTH = 1, WORLD = HEALTH << 1, RENDER = WORLD << 1,
	MOVEMENT = RENDER << 1, AI = MOVEMENT << 1,
};

struct HealthComponent : Component {
	const static COMPONENT_TYPE type = HEALTH;
	float currHP, maxHP;
	HealthComponent() : currHP(0.0f), maxHP(0.0f) {}
	HealthComponent( float hp, float max ) : currHP( hp ), maxHP( max ) {

	}
};

struct WorldComponent : Component {
	const static COMPONENT_TYPE type = WORLD;
	glm::vec2 pos, size;
	float rotation;
	SHAPE shape;
	WorldComponent() : pos( glm::vec2( 0, 0 ) ), size( glm::vec2( 10, 10 ) ), shape( RECTANGLE ) {}
	WorldComponent( glm::vec2 pos, glm::vec2 size, SHAPE shape ) :
		pos( pos ), size( size ), shape( shape ) {

	}
};

struct RenderComponent : Component {
	const static COMPONENT_TYPE type = RENDER;
	glm::vec3 color;
	GLuint textureID;
	RenderComponent() : textureID( 0 ) {}
	RenderComponent( GLuint tID ) :
		textureID( tID ) {

	}
};

struct MovementComponent : Component {
	const static COMPONENT_TYPE type = MOVEMENT;
	glm::vec2 vel;
	float defSpeed;
	MovementComponent() : vel( glm::vec2(0.0f) ), defSpeed(0.0f) {}
	MovementComponent( glm::vec2 vel, float speed ) :
		vel( vel ), defSpeed( speed ) {

	}
};

enum AI_TYPE { ENEMY };

struct AIComponent : Component {
	const static COMPONENT_TYPE type = AI;
	AI_TYPE AIType;
	glm::vec2 target;
	AIComponent() : target( glm::vec2( 0.0f ) ), AIType( ENEMY ) {}
	AIComponent( glm::vec2 target, AI_TYPE AIType ) :
		target(target), AIType( AIType ) {

	}
};

#endif // COMPONENT_H