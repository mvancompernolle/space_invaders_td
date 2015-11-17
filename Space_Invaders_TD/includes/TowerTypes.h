#pragma once

#ifndef TOWER_TYPES_H
#define TOWER_TYPES_H

enum TOWER_TYPE {
	TOWER_NONE, TOWER_WALL,
	TOWER_TRUE, TOWER_VOID, TOWER_PLASMA, TOWER_ICE,
	TOWER_TRUE_VOID, TOWER_TRUE_PLASMA, TOWER_TRUE_ICE,
	TOWER_VOID_TRUE, TOWER_VOID_PLASMA, TOWER_VOID_ICE,
	TOWER_PLASMA_TRUE, TOWER_PLASMA_VOID, TOWER_PLASMA_ICE,
	TOWER_ICE_TRUE, TOWER_ICE_VOID, TOWER_ICE_PLASMA
};

#endif // TOWER_TYPES_H