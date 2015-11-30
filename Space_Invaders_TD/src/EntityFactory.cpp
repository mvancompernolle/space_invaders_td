#include "EntityFactory.h"
#include "Consts.h"

World* EntityFactory::world;
CollisionSystem* EntityFactory::collisionSystem;
ShootSystem* EntityFactory::shootSystem;
DamageAuraSystem* EntityFactory::dmgAuraSystem;

EntityFactory::EntityFactory() {
}


EntityFactory::~EntityFactory() {
}

void EntityFactory::setWorld( World* w ) {
	world = w;
}

void EntityFactory::setCollisionSystem( CollisionSystem* cs ) {
	collisionSystem = cs;
}

void EntityFactory::setShootSystem( ShootSystem* ss ) {
	shootSystem = ss;
}

void EntityFactory::setDmgAuraSystem( DamageAuraSystem* das ) {
	dmgAuraSystem = das;
}

void EntityFactory::removeEntity( int pos ) {
	if ( world->entities[pos].mask == NONE ) {
		return;
	}

	for ( int i = 1; i < COMPONENT_SIZE; i <<= 1 ) {
		switch ( i & world->entities[pos].mask ) {
		case HEALTH:
			world->healthComponents.remove( world->getComponentIndex( pos, HEALTH ) );
			break;
		case WORLD:
			world->worldComponents.remove( world->getComponentIndex( pos, WORLD ) );
			break;
		case RENDER:
			world->renderComponents.remove( world->getComponentIndex( pos, RENDER ) );
			break;
		case MOVEMENT:
			world->movementComponents.remove( world->getComponentIndex( pos, MOVEMENT ) );
			break;
		case PATH:
			world->pathComponents.remove( world->getComponentIndex( pos, PATH ) );
			break;
		case SPAWN:
			world->spawnComponents.remove( world->getComponentIndex( pos, SPAWN ) );
			break;
		case COLLISION:
			if ( world->collisionComponents[world->getComponentIndex( pos, COLLISION )].collisionID != ENEMY ) {
				collisionSystem->unregisterEntity( pos );
			} else {
				collisionSystem->unregisterEnemy( pos );
			}
			world->collisionComponents.remove( world->getComponentIndex( pos, COLLISION ) );

			// remove entity from shoot system if an enemy
			shootSystem->unregisterEntity( pos );
			dmgAuraSystem->unregisterEntity( pos );
			break;
		case PLAYER_INPUT:
			world->playerInputComponents.remove( world->getComponentIndex( pos, PLAYER_INPUT ) );
			break;
		case DAMAGE:
			world->dmgComponents.remove( world->getComponentIndex( pos, DAMAGE ) );
			break;
		case MONEY:
			world->moneyComponents.remove( world->getComponentIndex( pos, MONEY ) );
			break;
		case SHOOT:
			world->shootComponents.remove( world->getComponentIndex( pos, SHOOT ) );
			break;
		case FOLLOW:
			world->followComponents.remove( world->getComponentIndex( pos, FOLLOW ) );
			break;
		case DAMAGE_AURA:
			world->dmgAuraComponents.remove( world->getComponentIndex( pos, DAMAGE_AURA ) );
			break;
		case SLOWED:
			world->slowComponents.remove( world->getComponentIndex( pos, SLOWED ) );
			break;
		case AOE:
			world->AOEComponents.remove( world->getComponentIndex( pos, AOE ) );
			break;
		case PARENT:
			world->parentComponents.remove( world->getComponentIndex( pos, PARENT ) );
			break;
		case ROTATION:
			world->rotationComponents.remove( world->getComponentIndex( pos, ROTATION ) );
			break;
		}
	}
	world->entities[pos].mask = NONE;
	world->entities.remove( pos );
}

int EntityFactory::createEntity( unsigned mask ) {
	int index = world->entities.create();
	world->entities[index].mask = mask;
	if ( mask & WORLD ) { addComponent(index, WORLD); }
	if ( mask & HEALTH ) { addComponent( index, HEALTH ); }
	if ( mask & RENDER ) { addComponent( index, RENDER ); }
	if ( mask & PATH ) { addComponent( index, PATH ); }
	if ( mask & MOVEMENT ) { addComponent( index, MOVEMENT ); }
	if ( mask & SPAWN ) { addComponent( index, SPAWN ); }
	if ( mask & PLAYER_INPUT ) { addComponent( index, PLAYER_INPUT ); }
	if ( mask & COLLISION ) { 
		addComponent( index, COLLISION );
		collisionSystem->registerEntity( index );
	}
	if ( mask & DAMAGE ) { addComponent( index, DAMAGE ); }
	if ( mask & MONEY ) { addComponent( index, MONEY ); }
	if ( mask & SHOOT ) { addComponent( index, SHOOT ); }
	if ( mask & FOLLOW ) { addComponent( index, FOLLOW ); }
	if ( mask & DAMAGE_AURA ) { addComponent( index, DAMAGE_AURA ); }
	if ( mask & SLOWED ) { addComponent( index, SLOWED ); }
	if ( mask & AOE ) { addComponent( index, AOE ); }
	if ( mask & PARENT ) { addComponent( index, PARENT ); }
	if ( mask & ROTATION ) { addComponent( index, ROTATION ); }
	return index;
}

int EntityFactory::createEnemy() {
	int index = world->entities.create();
	world->entities[index].mask = ( HEALTH | RENDER | WORLD | MOVEMENT | PATH | COLLISION );
	addComponent( index, HEALTH );
	addComponent( index, RENDER );
	addComponent( index, WORLD );
	addComponent( index, MOVEMENT );
	addComponent( index, PATH );
	addComponent( index, COLLISION );
	collisionSystem->registerEntity( index );

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.pos = glm::vec2( 0 );
	worldComp.size = glm::vec2( 32 );
	MovementComponent& movComp = world->movementComponents[world->getComponentIndex( index, MOVEMENT )];
	movComp.vel = glm::vec2( 200.0f, 0.0f );
	movComp.defSpeed = 200.0f;
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "enemy";
	HealthComponent& healthComp = world->healthComponents[world->getComponentIndex( index, HEALTH )];
	healthComp.currHP = 100.0f;
	healthComp.maxHP = 100.0f;
	CollisionComponent& collComp = world->collisionComponents[world->getComponentIndex( index, COLLISION )];
	collComp.shape = CIRCLE;
	collComp.collisionID = ENEMY;
	collComp.collisionMask = BULLET;

	return index;
}

int EntityFactory::createBaseTower() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | MONEY | ROTATION );
	addComponent( index, MONEY );
	addComponent( index, RENDER );
	addComponent( index, WORLD );
	addComponent( index, ROTATION );

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.rotation = rand() % 360;
	worldComp.size = glm::vec2( 64 );
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "tower_base";
	MoneyComponent& moneyComp = world->moneyComponents[world->getComponentIndex( index, MONEY )];
	moneyComp.value = 5;
	RotationComponent& rotComp = world->rotationComponents[world->getComponentIndex( index, ROTATION )];
	rotComp.rotSpeed = (15 + rand() % 30) * (rand() % 2 == 0 ? 1 : -1);

	return index;
}

int EntityFactory::createPlayer() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | MOVEMENT | PLAYER_INPUT );
	addComponent( index, PLAYER_INPUT );
	addComponent( index, RENDER );
	addComponent( index, WORLD );
	addComponent( index, MOVEMENT );

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.rotation = 0.0f;
	worldComp.size = glm::vec2( 32 );
	worldComp.pos = glm::vec2( GAME_WIDTH * 0.5f - worldComp.size.x / 2.0f, GAME_HEIGHT * 0.7f );
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "player";
	MovementComponent& movComp = world->movementComponents[world->getComponentIndex( index, MOVEMENT )];
	movComp.vel = glm::vec2( 0.0f, 0.0f );
	movComp.defSpeed = 200.0f;

	return index;
}

int EntityFactory::createSpawner() {
	int index = world->entities.create();
	world->entities[index].mask = ( RENDER | WORLD | SPAWN | ROTATION );
	addComponent( index, SPAWN );
	addComponent( index, RENDER );
	addComponent( index, WORLD );
	addComponent( index, ROTATION );

	// initialize values
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( index, WORLD )];
	worldComp.rotation = 0.0f;
	worldComp.size = glm::vec2( 32 );
	RenderComponent& renderComp = world->renderComponents[world->getComponentIndex( index, RENDER )];
	renderComp.color = glm::vec3( 1.0f );
	renderComp.textureName = "portal";
	SpawnComponent& spawnComp = world->spawnComponents[world->getComponentIndex( index, SPAWN )];
	spawnComp.round = -1;
	spawnComp.numRounds = 20;
	world->rotationComponents[world->getComponentIndex( index, ROTATION )].rotSpeed = 90.0f;

	return index;
}

unsigned EntityFactory::addEntity( Entity ent ) {
	int entPos = EntityFactory::createEntity( ent.componentTypes );
	for ( int j = 1; j < COMPONENT_SIZE; j <<= 1 ) {
		switch ( j & ent.componentTypes ) {
		case HEALTH:
			world->healthComponents[world->getComponentIndex( entPos, HEALTH )] = ent.health;
			break;
		case WORLD:
			world->worldComponents[world->getComponentIndex( entPos, WORLD )] = ent.world;
			break;
		case RENDER:
			world->renderComponents[world->getComponentIndex( entPos, RENDER )] = ent.render;
			break;
		case MOVEMENT:
			world->movementComponents[world->getComponentIndex( entPos, MOVEMENT )] = ent.movement;
			break;
		case PATH:
			world->pathComponents[world->getComponentIndex( entPos, PATH )] = ent.path;
			break;
		case SPAWN:
			world->spawnComponents[world->getComponentIndex( entPos, SPAWN )] = ent.spawn;
			break;
		case COLLISION:
			world->collisionComponents[world->getComponentIndex( entPos, COLLISION )] = ent.collision;
			if ( ent.collision.collisionID == ENEMY ) {
				// make sure the enemy is in its own collision list
				collisionSystem->unregisterEntity( entPos );
				collisionSystem->registerEnemy( entPos );

				shootSystem->registerEntity( entPos );
				dmgAuraSystem->registerEntity( entPos );
			}
			break;
		case PLAYER_INPUT:
			world->playerInputComponents[world->getComponentIndex( entPos, PLAYER_INPUT )] = ent.playerInput;
			break;
		case DAMAGE:
			world->dmgComponents[world->getComponentIndex( entPos, DAMAGE )] = ent.damage;
			break;
		case MONEY:
			world->moneyComponents[world->getComponentIndex( entPos, MONEY )] = ent.money;
			break;
		case SHOOT:
			world->shootComponents[world->getComponentIndex( entPos, SHOOT )] = ent.shoot;
			break;
		case FOLLOW:
			world->followComponents[world->getComponentIndex( entPos, FOLLOW )] = ent.follow;
			break;
		case DAMAGE_AURA:
			world->dmgAuraComponents[world->getComponentIndex( entPos, DAMAGE_AURA )] = ent.dmgAura;
			break;
		case SLOWED:
			world->slowComponents[world->getComponentIndex( entPos, SLOWED )] = ent.slowed;
			break;
		case AOE:
			world->AOEComponents[world->getComponentIndex( entPos, AOE )] = ent.aoe;
			break;
		case PARENT:
			world->parentComponents[world->getComponentIndex( entPos, PARENT )] = ent.parent;
			break;
		case ROTATION:
			world->rotationComponents[world->getComponentIndex( entPos, ROTATION )] = ent.rotation;
			break;
		}
	}
	return entPos;
}

unsigned EntityFactory::addComponent( unsigned pos, COMPONENT_TYPE type ) {
	int index = -1;
	// return -1 if trying to add a component to an entity that doesn't exist
	if ( world->entities[pos].mask == NONE ) {
		return index;
	}

	switch ( type ) {
	case HEALTH:
		index = world->healthComponents.create();
		break;
	case WORLD:
		index = world->worldComponents.create();
		break;
	case RENDER:
		index = world->renderComponents.create();
		break;
	case MOVEMENT:
		index = world->movementComponents.create();
		break;
	case PATH:
		index = world->pathComponents.create();
		break;
	case SPAWN:
		index = world->spawnComponents.create();
		break;
	case COLLISION:
		index = world->collisionComponents.create();
		break;
	case PLAYER_INPUT:
		index = world->playerInputComponents.create();
		break;
	case DAMAGE:
		index = world->dmgComponents.create();
		break;
	case MONEY:
		index = world->moneyComponents.create();
		break;
	case SHOOT:
		index = world->shootComponents.create();
		break;
	case FOLLOW:
		index = world->followComponents.create();
		break;
	case DAMAGE_AURA:
		index = world->dmgAuraComponents.create();
		break;
	case SLOWED:
		index = world->slowComponents.create();
		break;
	case AOE:
		index = world->AOEComponents.create();
		break;
	case PARENT:
		index = world->parentComponents.create();
		break;
	case ROTATION:
		index = world->rotationComponents.create();
		break;
	}
	world->setComponentIndex( pos, type, index );
	world->entities[pos].mask |= type;
	//resetComponent( pos, type );
	return index;
}

void EntityFactory::resetComponent( unsigned pos, COMPONENT_TYPE type ) {
	if ( !(world->entities[pos].mask & type) ) {
		return;
	}
	//std::cout << "resetting" << std::endl;
	switch ( type ) {
	case HEALTH:
		world->healthComponents[world->getComponentIndex( pos, HEALTH )] = HealthComponent();
		break;
	case WORLD:
		world->worldComponents[world->getComponentIndex( pos, WORLD )] = WorldComponent();
		break;
	case RENDER:
		world->renderComponents[world->getComponentIndex( pos, RENDER )] = RenderComponent();
		break;
	case MOVEMENT:
		world->movementComponents[world->getComponentIndex( pos, MOVEMENT )] = MovementComponent();
		break;
	case PATH:
		world->pathComponents[world->getComponentIndex( pos, PATH )] = PathAIComponent();
		break;
	case SPAWN:
		world->spawnComponents[world->getComponentIndex( pos, SPAWN )] = SpawnComponent();
		break;
	case COLLISION:
		world->collisionComponents[world->getComponentIndex( pos, COLLISION )] = CollisionComponent();
		break;
	case PLAYER_INPUT:
		world->playerInputComponents[world->getComponentIndex( pos, PLAYER_INPUT )] = PlayerInputComponent();
		break;
	case DAMAGE:
		world->dmgComponents[world->getComponentIndex( pos, DAMAGE )] = DamageComponent();
		break;
	case MONEY:
		world->moneyComponents[world->getComponentIndex( pos, MONEY )] = MoneyComponent();
		break;
	case SHOOT:
		world->shootComponents[world->getComponentIndex( pos, SHOOT )] = ShootComponent();
		break;
	case FOLLOW:
		world->followComponents[world->getComponentIndex( pos, FOLLOW )] = FollowComponent();
		break;
	case DAMAGE_AURA:
		world->dmgAuraComponents[world->getComponentIndex( pos, DAMAGE_AURA )] = DmgAuraComponent();
		break;
	case SLOWED:
		world->slowComponents[world->getComponentIndex( pos, SLOWED )] = SlowedComponent();
		break;
	case AOE:
		world->AOEComponents[world->getComponentIndex( pos, AOE )] = AOEComponent();
		break;
	case PARENT:
		world->parentComponents[world->getComponentIndex( pos, PARENT )] = ParentComponent();
		break;
	case ROTATION:
		world->rotationComponents[world->getComponentIndex( pos, ROTATION )] = RotationComponent();
		break;
	}
}