#include "TankEnemy.h"

TankEnemy::TankEnemy(Point2D position) :
	Enemy(101, position, 60, "tank_south")
{
	BaseSprite = "tank";
	CurrentDirection = Direction::SOUTH;
	AnimSpeed = 0.1f;

	Speed = 1.0f;
	DetectionRange = 300;

	Health = 5.0f;

	AttackDamage = 3.0f;
	AttackCooldown = 1.0f;
	//CurrentAttackCooldown = AttackCooldown;
	AttackRange = 250;
	ProjectileSpeed = 2.0f;

	GameObjectRef.animSpeed = AnimSpeed;
	GameObjectRef.scale = 2.0f;
}
