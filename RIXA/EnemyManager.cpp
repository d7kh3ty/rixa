#include "EnemyManager.h"
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

#include "Camera.h"
#include "Character.h"
#include "Enemy.h"
void EnemyManager::UpdateEnemies(float deltaTime)
{
	std::vector<int> eVector = Play::CollectGameObjectIDsByType(101);
	Camera* camera = Play::GetCamera();

	for (int eID : eVector)
	{
		GameObject& enemyGameObject = Play::GetGameObject(eID);
		Character* enemy = enemyGameObject.owningCharacter;

		enemy->Update(deltaTime);

	}
}

void EnemyManager::DeleteAllEnemies()
{
	std::vector<int> eVector = Play::CollectGameObjectIDsByType(101);

	for (int eID : eVector)
	{
		GameObject& enemyGameObject = Play::GetGameObject(eID);
		Character* enemy = enemyGameObject.owningCharacter;

		delete enemy;
		Play::DestroyGameObject(eID);
	}
}
