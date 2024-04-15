#include "Player.h"
#include "../Camera/Camera.h"
#include "PlayerProjectile.h"
#include "../Manager/ParticleManager.h"

// Player designated 2 id and a radius of 33
Player::Player(const Point2D pos) :
	Character(0, pos, 33, "angel_walk_north"), // Change this to east later
	AngularSpeedModifier(0.7f),
	DamagedCooldown(0.1f),
	CurrentDamagedCooldown(DamagedCooldown),
	IsDamaged(true)
{
	BaseSprite = "angel_walk";
	CurrentDirection = Direction::NORTH;
	AnimSpeed = 0.07f;

	Speed = 4;
	ProjectileSpeed = 11;

	AttackDamage = 1.0f;
	AttackRange = 300.0f;
	AttackCooldown = 0.1f;
	CurrentAttackCooldown = AttackCooldown;
	Health = 5;
}

void Player::Update(float deltaTime)
{
	HandlePlayerMovement();
	HandlePlayerShooting(deltaTime);
	UpdateDirection();
	CheckLevelCollision();
	UpdateDamaged(deltaTime);
}

// I can make this a lot better but leave it for now
void Player::OnRecieveDamage()
{
	// Sounds
	Play::PlayAudio("playerhit");

	// Reset attributes
	CurrentDamagedCooldown = DamagedCooldown;
	IsDamaged = true;
	SetPlayerColour({ 255,0,0 });
}

void Player::UpdateDamaged(float deltaTime)
{
	if (IsDamaged)
	{
		CurrentDamagedCooldown -= deltaTime;
	}
	if (CurrentDamagedCooldown <= 0)
	{
		IsDamaged = false;
		CurrentDamagedCooldown = DamagedCooldown;
		SetPlayerColour({ 100,100,100 });
	}
}

void Player::Shoot()
{
	Point2D position = GameObjectRef.pos;
	Camera* camera = Play::GetCamera();

	// Find x and y of mouse relative to position
	Point2D mousePos = Play::GetMousePos();
	int x = (mousePos.x + camera->GetXOffset()) - position.x;
	int y = (mousePos.y + camera->GetYOffset()) - position.y;

	float length = sqrt(x * x + y * y) / ProjectileSpeed;

	PlayerProjectile* playerProjectile = new PlayerProjectile(*this, Vector2D(x / length, y / length));

	playerProjectile->GetGameObject().animSpeed = 0.075f;

	CurrentAttackCooldown = AttackCooldown;

	OnShoot();
}

// For any sounds or mechanics that occur on shoot
void Player::OnShoot()
{
	Play::PlayAudio("playershoot");
}



// Blow up
void Player::OnDeath()
{
	// EXPLOSION?
	Play::PlayAudio("explode");
	ParticleManager::AddParticle("exp_bubble", GameObjectRef.pos, 0.2f, 2.0f);
	SetIsDead(true);
}

// If collides on level obstacle, do not move.
void Player::OnLevelCollision()
{
	GameObjectRef.pos = GameObjectRef.oldPos;
}

void Player::UpdateDirection()
{
	// If direction changed from last frame, we change it
	Direction oldDirection = CurrentDirection;

	float xVel = GameObjectRef.velocity.x;
	float yVel = GameObjectRef.velocity.y;

	if (yVel < 0) // North
	{
		if (xVel > 0) // North east
		{
			CurrentDirection = Direction::NORTHEAST;
		}
		else if (xVel < 0) // North west
		{
			CurrentDirection = Direction::NORTHWEST;
		}
		else
		{
			CurrentDirection = Direction::NORTH;
		}
	}
	else if (yVel > 0) // South
	{
		if (xVel > 0) // South east
		{
			CurrentDirection = Direction::SOUTHEAST;
		}
		else if (xVel < 0) // South west
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
		if (xVel > 0)
		{
			CurrentDirection = Direction::EAST;
		}
		else if (xVel < 0)
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

// These can be updated to be more actions based
// Movement controls
void Player::HandlePlayerMovement()
{
	float xVel = 0;
	float yVel = 0;

	// Modifier velocity
	if (Play::KeyDown(0x57)) { // W
		yVel -= Speed;
	}
	if (Play::KeyDown(0x44)) { // D
		xVel += Speed;
	}
	if (Play::KeyDown(0x53)) { // S
		yVel += Speed;
	}
	if (Play::KeyDown(0x41)) { // A
		xVel -= Speed;
	}

	// If both non-zero, apply a speed modifier
	if ((xVel != 0) && (yVel != 0))
	{
		xVel *= AngularSpeedModifier;
		yVel *= AngularSpeedModifier;
	}

	// If both zero, pause animation
	if ((xVel == 0) && (yVel == 0)) {
		GameObjectRef.animSpeed = 0;

	}
	else
	{
		GameObjectRef.animSpeed = AnimSpeed;
	}

	// Apply resulting velocity
	GameObjectRef.velocity = { xVel , yVel  };


}

// Shooting controls
void Player::HandlePlayerShooting(float deltaTime)
{
	if (CurrentAttackCooldown > 0)
	{
		CurrentAttackCooldown -= deltaTime;
		return;
	}

	if (Play::KeyPressed(VK_LBUTTON))
	{
		Shoot();
	}
}

void Player::SetPlayerColour(Play::Colour colour)
{
	Play::ColourSprite("angel_walk_north", colour);
	Play::ColourSprite("angel_walk_northeast", colour);
	Play::ColourSprite("angel_walk_northwest", colour);
	Play::ColourSprite("angel_walk_west", colour);
	Play::ColourSprite("angel_walk_south", colour);
	Play::ColourSprite("angel_walk_southwest", colour);
	Play::ColourSprite("angel_walk_southeast", colour);
	Play::ColourSprite("angel_walk_east", colour);
}
