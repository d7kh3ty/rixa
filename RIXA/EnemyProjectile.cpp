#include "EnemyProjectile.h"
#include "Player.h"

EnemyProjectile::EnemyProjectile(Character* owner, Point2D velocity) :
	Projectile(owner, "enemy_bullet", velocity)
{
	// Bullet size depends on the damage
	GameObjectRef.scale = 0.5f * owner->GetAttackDamage();
}

// Checking for player collision
bool EnemyProjectile::CheckCollision()
{
	Player* player = Play::GetPlayer();
	GameObject& playerGameObject = player->GetGameObject();

	// If bullet is colliding with the player
	if (Play::IsColliding(playerGameObject, GameObjectRef))
	{
		// Apply damage
		player->ApplyDamage(Owner->GetAttackDamage());

		return true;
	}
	return false;
}
