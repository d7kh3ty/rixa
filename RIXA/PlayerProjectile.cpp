#include "PlayerProjectile.h"
#include "Camera.h"
#include "Character.h"
PlayerProjectile::PlayerProjectile(Character* owner, Point2D pos) :
	Projectile(owner, "bullet", pos)
{
	GameObjectRef.animSpeed = 0.075f;
}

// Needs a slightly modified update
void PlayerProjectile::Update(float deltaTime)
{
	// Draw projectile
	Camera* camera = Play::GetCamera();
	camera->DrawOffset(&GameObjectRef);

	// If cannot damage and animation completed, destroy
	if (Play::IsAnimationComplete(GameObjectRef))
	{
		Play::DestroyGameObject(GameObjectID);
		delete this;
		return;
	}

	if (!CanDamage)
	{
		return;
	}

	// If out of range, velocity is 0 and can no longer damage
	if (CheckOutOfRange())
	{
		GameObjectRef.velocity = { 0,0 };
		CanDamage = false;
		return;
	}

	if (CheckCollision())
	{
		Play::DestroyGameObject(GameObjectID);
		delete this;
		return;
	}
}

bool PlayerProjectile::CheckCollision()
{
	std::vector<int> eVector = Play::CollectGameObjectIDsByType(101);

	for (int eID : eVector)
	{
		GameObject& enemyGameObject = Play::GetGameObject(eID);

		if (Play::IsColliding(enemyGameObject, GameObjectRef))
		{
			Character* enemy = enemyGameObject.owningCharacter;
			enemy->ApplyDamage(Owner->GetAttackDamage());
			return true;
		}
	}
	return false;
}
