#include "DroneEnemy.h"

DroneEnemy::DroneEnemy(Point2D position) :
	Enemy(101, position, 15, "cute_south")

{
	BaseSprite = "cute";
	CurrentDirection = Direction::SOUTH;
	AnimSpeed = 0.1f;

	Speed = 3.0f;
	DetectionRange = 300;

	Health = 1.0f;

	AttackDamage = 1.0f;
	AttackCooldown = 1.0f;
	//CurrentAttackCooldown = AttackCooldown;
	AttackRange = 250.0f;
	ProjectileSpeed = 1.5f;

	GameObjectRef.animSpeed = AnimSpeed;
	GameObjectRef.scale = 0.5f;
}

