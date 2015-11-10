#include "PlayerInputSystem.h"
#include "ServiceLocator.h"
#include "EntityFactory.h"
#include "consts.h"

PlayerInputSystem::PlayerInputSystem()
{
	flags = ( PLAYER_INPUT | MOVEMENT | WORLD );
}


PlayerInputSystem::~PlayerInputSystem()
{
}

void PlayerInputSystem::update( World* world, int pos, float dt ) {
	WorldComponent& worldComp = world->worldComponents[world->getComponentIndex( pos, WORLD )];
	MovementComponent& moveComp = world->movementComponents[world->getComponentIndex( pos, MOVEMENT )];
	PlayerInputComponent& inputComp = world->playerInputComponents[world->getComponentIndex( pos, PLAYER_INPUT )];

	// handle movements left
	bool found = false;
	int i = 0;
	while ( !found && i < inputComp.leftInputs.size() ) {
		if ( ServiceLocator::getInput().getKeyPressed( inputComp.leftInputs[i] ) ){
			found = true;
		}
		i++;
	}
	if ( found ) {
		moveComp.vel = glm::vec2( -moveComp.defSpeed, 0.0f );
	} else if ( moveComp.vel.x < 0.0f ) {
		moveComp.vel = glm::vec2( 0.0f, 0.0f );
	}

	// handle movements right
	found = false;
	i = 0;
	while ( !found && i < inputComp.rightInputs.size() ) {
		if ( ServiceLocator::getInput().getKeyPressed( inputComp.rightInputs[i] ) ) {
			found = true;
		}
		i++;
	}
	if ( found ) {
		moveComp.vel = glm::vec2( moveComp.defSpeed, 0.0f );
	} else if ( moveComp.vel.x > 0.0f ) {
		moveComp.vel = glm::vec2( 0.0f, 0.0f );
	}

	// handle shoot commands
	i = 0;
	while ( i < inputComp.shootPrimaryInputs.size() ) {
		if ( ServiceLocator::getInput().getKeyPressed( inputComp.shootPrimaryInputs[i] ) ) {
			// shoot bullet
			int index = EntityFactory::createEntity( ( WORLD | RENDER | MOVEMENT | COLLISION | DAMAGE ) );
			WorldComponent& bWorld = world->worldComponents[world->getComponentIndex( index, WORLD )];
			bWorld.rotation = 0.0f;
			bWorld.size = glm::vec2( 16 );
			bWorld.pos = glm::vec2( worldComp.pos + worldComp.size / 2.0f - bWorld.size / 2.0f );
			RenderComponent& bRender = world->renderComponents[world->getComponentIndex( index, RENDER )];
			bRender.color = glm::vec3( 1.0f );
			bRender.textureName = "ship_true_dmg_bullet";
			MovementComponent& bMove = world->movementComponents[world->getComponentIndex( index, MOVEMENT )];
			bMove.defSpeed = 1200.0f;
			bMove.vel = glm::normalize( ServiceLocator::getInput().getMousePos() - ( worldComp.pos + ( worldComp.size / 2.0f ) ) ) * bMove.defSpeed + moveComp.vel;
			CollisionComponent& bCollision = world->collisionComponents[world->getComponentIndex( index, COLLISION )];
			bCollision.shape = CIRCLE;
			bCollision.collisionID = BULLET;
			bCollision.collisionMask = ( ENEMY | WALL );
			DamageComponent& bDmg = world->dmgComponents[world->getComponentIndex( index, DAMAGE )];
			bDmg.trueDmg = 5.0f;
			bDmg.iceDmg = 1.0f;
			bDmg.plasmaDmg = 1.0f;
			bDmg.voidDmg = 1.0f;
			break;
		}
		i++;
	}
}
