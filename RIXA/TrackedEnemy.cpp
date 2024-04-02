#include "TrackedEnemy.h"

TrackedEnemy::TrackedEnemy(Point2D position) :
	Enemy(101, position, 30, "tracked_south")
{
	BaseSprite = "tracked";
	CurrentDirection = Direction::SOUTH;
	AnimSpeed = 0.1f;

	Speed = 1.5f;
	DetectionRange = 300;

	Health = 4.0f;

	AttackDamage = 2.0f;
	AttackCooldown = 0.5f;
	//CurrentAttackCooldown = AttackCooldown;
	AttackRange = 250;
	ProjectileSpeed = 2.5f;

	GameObjectRef.animSpeed = AnimSpeed;
	GameObjectRef.scale = 0.7f;
}
