#include "Enemy.h"
#include "EnemyProjectile.h"
#include "Player.h"
#include "../Level/Level.h"
#include "../Camera/Camera.h"
#include "../Helper.h"
#include "../Manager/ParticleManager.h"
Enemy::Enemy(int type, const Point2D pos, int collisionRadius, const char* spriteName) :
	Character(type, pos, collisionRadius, spriteName),
	DetectionRange(250),
	PlayerDetected(false),
	DirectionTolerance(40.0f)
{
	GameObjectRef.owningEnemy = this;
}

void Enemy::Update(float deltaTime)
{
	// Draw enemy
	Camera* camera = Play::GetCamera();
	camera->DrawOffset(GameObjectRef);

	// This is to shoot and calculate distance
	Player* player = Play::GetPlayer();
	GameObject& playerGameObject = player->GetGameObject();

	float xDifference = playerGameObject.pos.x - GameObjectRef.pos.x;
	float yDifference = playerGameObject.pos.y - GameObjectRef.pos.y;
	float distanceToPlayer = sqrt((xDifference * xDifference) + (yDifference * yDifference));

	// If player is not detected, check again and return
	if (!PlayerDetected) 
	{
		// Get the player with edited playbuffer code
		PlayerDetected = distanceToPlayer < DetectionRange;
		return;
	}

	// If the player is detected, the following behaviours will run
	// Check for collisions with the environment
	CheckLevelCollision();

	// Shooting
	if (CanShoot(deltaTime))
	{
		// Shoot and move
		float xSpeed = (xDifference / distanceToPlayer) * ProjectileSpeed;
		float ySpeed = (yDifference / distanceToPlayer) * ProjectileSpeed;
		TryShoot(Point2D(xSpeed, ySpeed));
		UpdateMovement();
	}

	//// Update direction
	UpdateDirection();

}

void Enemy::OnRecieveDamage()
{
	ParticleManager::AddParticle("exp_pop", GameObjectRef.pos, 0.2f, 1.0f);
	Play::PlayAudio("enemyhit");
}

void Enemy::OnDeath()
{
	ParticleManager::AddParticle("exp_pop", GameObjectRef.pos, 0.2f, 1.75f);
	Play::PlayAudio("synthetic_bomb");
	GetGameObject().velocity = { 0,0 };
	Play::DestroyGameObject(GetID());
	SetIsDead(true);
}

void Enemy::OnLevelCollision()
{
	GameObjectRef.velocity = -GameObjectRef.velocity;
	return;
}

// Update directions
void Enemy::UpdateDirection()
{
	// If direction changed from last frame, we change it
	Direction oldDirection = CurrentDirection;

	GameObject& playerGameObject = Play::GetPlayer()->GetGameObject();

	float xDifference = playerGameObject.pos.x - GameObjectRef.pos.x;
	float yDifference = playerGameObject.pos.y - GameObjectRef.pos.y;

	if (yDifference < -DirectionTolerance) // North
	{
		if (xDifference > DirectionTolerance) // North east
		{
			CurrentDirection = Direction::NORTHEAST;
		}
		else if (xDifference < -DirectionTolerance) // North west
		{
			CurrentDirection = Direction::NORTHWEST;
		}
		else
		{
			CurrentDirection = Direction::NORTH;
		}
	}
	else if (yDifference > DirectionTolerance) // South
	{
		if (xDifference > DirectionTolerance) // South east
		{
			CurrentDirection = Direction::SOUTHEAST;
		}
		else if (xDifference < -DirectionTolerance) // South west
		{
			CurrentDirection = Direction::SOUTHWEST;
		}
		else // South
		{
			CurrentDirection = Direction::SOUTH;
		}
	}
	else // If yVel = 0
	{
		if (xDifference > DirectionTolerance)
		{
			CurrentDirection = Direction::EAST;
		}
		else if (xDifference < -DirectionTolerance)
		{
			CurrentDirection = Direction::WEST;
		}
	}

	// If direction is changed, change sprite
	if (CurrentDirection != oldDirection)
	{
		OnDirectionChange();
	}
}

// Check if off cooldown
bool Enemy::CanShoot(float deltaTime)
{
	// If on cooldown, decrement cooldown
	if (CurrentAttackCooldown > 0)
	{
		CurrentAttackCooldown -= deltaTime;
		return false;
	}

	// Otherwise can shoot
	CurrentAttackCooldown = AttackCooldown;
	return true;
}

// Shoot
void Enemy::TryShoot(const Point2D velocity)
{
	new EnemyProjectile(*this, velocity);

}

// Update movement, runs after each shot
void Enemy::UpdateMovement()
{
	if (Speed <= 0)
	{
		return;
	}

	GameObject& playerGameObject = Play::GetPlayer()->GetGameObject();

	float xDifference = playerGameObject.pos.x - GameObjectRef.pos.x;
	float yDifference = playerGameObject.pos.y - GameObjectRef.pos.y;

	// Move roughly towards the player 
	float velocityX = xDifference + Play::RandomRollRange(-300, 300);
	float velocityY = yDifference + Play::RandomRollRange(-300, 300);
	float magnitude = sqrt(velocityX * velocityX + velocityY * velocityY) / Speed;

	GameObjectRef.velocity = { ((velocityX) / magnitude) , ((velocityY) / magnitude) };

}

