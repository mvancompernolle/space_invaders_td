#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Component {

};

enum SHAPE { RECTANGLE, CIRCLE };
enum COMPONENT_TYPE { HEALTH, WORLD, RENDER };

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
	RenderComponent( const GLuint tID ) :
		textureID( tID ) {

	}
};

#endif // COMPONENT_H