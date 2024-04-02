#include "ProjectileManager.h"
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

#include "Projectile.h"
void ProjectileManager::UpdateProjectiles(float deltaTime)
{
	std::vector<int> pVector = Play::CollectGameObjectIDsByType(1);

	for (int pID : pVector)
	{
		GameObject& projectileGameObject = Play::GetGameObject(pID);
		Projectile* projectile = projectileGameObject.owningProjectile;

		projectile->Update(deltaTime);
	}
}

void ProjectileManager::DeleteAllProjectiles()
{
	std::vector<int> pVector = Play::CollectGameObjectIDsByType(1);

	for (int pID : pVector)
	{
		GameObject& projectileGameObject = Play::GetGameObject(pID);
		Projectile* projectile = projectileGameObject.owningProjectile;

		delete projectile;
		Play::DestroyGameObject(pID);
	}

}
