#include "TurretEnemy.h"

TurretEnemy::TurretEnemy(Point2D position) :
	Enemy(101, position, 30, "turret_south")
{
	BaseSprite = "turret";
	CurrentDirection = Direction::SOUTH;
	AnimSpeed = 0.1f;

	Speed = 0;
	DetectionRange = 400;

	Health = 3.0f;

	AttackDamage = 1.0f;
	AttackCooldown = 0.4f;
	//CurrentAttackCooldown = AttackCooldown;
	AttackRange = 350;
	ProjectileSpeed = 1.5f;

	GameObjectRef.animSpeed = AnimSpeed;
	GameObjectRef.scale = 0.6f;

}
