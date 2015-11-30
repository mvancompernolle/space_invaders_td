#include "systems/PlayerInputSystem.h"
#include "ServiceLocator.h"
#include "EntityFactory.h"
#include "consts.h"

PlayerInputSystem::PlayerInputSystem( unsigned* enemiesLeft ) : System( enemiesLeft )
{
	flags = ( PLAYER_INPUT | MOVEMENT | WORLD );
	systemName = "playerInputSystem";
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

	// keep player from going off screen
	if ( ( worldComp.pos.x < 0 && moveComp.vel.x < 0 ) || ( worldComp.pos.x + worldComp.size.x > GAME_WIDTH && moveComp.vel.x > 0 ) ) {
		moveComp.vel.x = 0;
	}

	// handle shoot commands
	inputComp.leftClickDT += dt;
	if ( inputComp.leftClickDT >= inputComp.leftClickCooldown ) {
		inputComp.leftClickDT -= inputComp.leftClickCooldown;

		i = 0;
		while ( i < inputComp.shootPrimaryInputs.size() ) {
			if ( ServiceLocator::getInput().getKeyPressed( inputComp.shootPrimaryInputs[i] ) ) {
				// shoot bullet
				int index = EntityFactory::createEntity( WORLD | RENDER | MOVEMENT | COLLISION | DAMAGE );
				WorldComponent& bWorld = world->worldComponents[world->getComponentIndex( index, WORLD )];
				bWorld.rotation = 0.0f;
				bWorld.size = glm::vec2( 16 );
				bWorld.pos = glm::vec2( worldComp.pos + worldComp.size / 2.0f - bWorld.size / 2.0f );
				RenderComponent& bRender = world->renderComponents[world->getComponentIndex( index, RENDER )];
				bRender.color = glm::vec3( 1.0f );
				bRender.textureName = "bullet_true";
				MovementComponent& bMove = world->movementComponents[world->getComponentIndex( index, MOVEMENT )];
				bMove.defSpeed = 1200.0f;
				bMove.vel = glm::normalize( ServiceLocator::getInput().getMousePos() - ( worldComp.pos + ( worldComp.size / 2.0f ) ) ) * bMove.defSpeed + moveComp.vel;
				CollisionComponent& bCollision = world->collisionComponents[world->getComponentIndex( index, COLLISION )];
				bCollision.shape = CIRCLE;
				bCollision.collisionID = BULLET;
				bCollision.collisionMask = ( ENEMY | WALL );
				DamageComponent& bDmg = world->dmgComponents[world->getComponentIndex( index, DAMAGE )];
				bDmg.trueDmg = 25.0f;
				if ( EntityFactory::collisionSystem->registeredEnemies.size() > 0 ) {
					unsigned followIndex = EntityFactory::addComponent( index, FOLLOW );
					FollowComponent& bFollow = world->followComponents[followIndex];
					bFollow.turnRate = 180.0f;
					int targetIndex = 0;
					glm::vec2 mousePos = ServiceLocator::getInput().getMousePos();
					std::vector<unsigned>& enemies = EntityFactory::collisionSystem->registeredEnemies;
					float dist = glm::distance( mousePos, world->worldComponents[world->getComponentIndex(enemies[0], WORLD)].pos);
					for ( int i = 1; i < enemies.size(); ++i ) {
						float newDist = glm::distance( mousePos, world->worldComponents[world->getComponentIndex( enemies[i], WORLD )].pos );
						if ( newDist < dist ) {
							dist = newDist;
							targetIndex = i;
						}
					}
					bFollow.entTarget = enemies[targetIndex];
				} 
				break;
			}
			i++;
		}
	}
}
