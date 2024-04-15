#include "Projectile.h"
#include "Character.h"
#include "../Helper.h"
#include "../Camera/Camera.h"

Projectile::Projectile(Character& owner, const char* asset, const Point2D velocity) :
	Actor(1, { owner.GetGameObject().pos.x , owner.GetGameObject().pos.y }, 1, asset),
	Owner(owner),
	CanDamage(true)
{
	// Set owning projectile on GO
	GameObjectRef.owningProjectile = this;

	// Set velocity
	GameObjectRef.velocity = velocity;
}

// Handles collision checks
void Projectile::Update(float deltaTime)
{
	// Draw if not destroyed
	Camera* camera = Play::GetCamera();
	camera->DrawOffset(GameObjectRef);

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

	// Check out of range or hits something or owner dies
	if (CheckOutOfRange() || CheckCollision() || Owner.GetIsDead())
	{
		GameObjectRef.animSpeed = 0.1f;
		GameObjectRef.velocity = { 0,0 };
		CanDamage = false;
		return;
	}


}

// Check for out of bounds or out of character's attack range
bool Projectile::CheckOutOfRange() const
{
	GameObject& ownerGameObject = Owner.GetGameObject();

	float attackRange = Owner.GetAttackRange();

	bool outOfRange = Helper::Distance(&GameObjectRef, &ownerGameObject) > attackRange;
	return outOfRange;

}

bool Projectile::CheckCollision()
{
	return false;
}
