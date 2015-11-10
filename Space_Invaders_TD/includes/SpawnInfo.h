#pragma once

#ifndef SPAWN_INFO_H
#define SPAWN_INFO_H

class Entity;

class SpawnInfo {
public: 
	float spawnRate;
	unsigned num;

	SpawnInfo();
	~SpawnInfo();
	void setEntity( const Entity* ent );
	const Entity* getEntity() const;
private:
	Entity* spawnType;
};

#endif SPAWN_INFO_H
