#include "ParticleManager.h"
#include "../Camera/Camera.h"

void ParticleManager::AddParticle(const char* asset, Point2D pos, float animSpeed, float scale)
{
	int pID = Play::CreateGameObject(9, pos, 0, asset);
	GameObject& particleGameObject = Play::GetGameObject(pID);

	particleGameObject.animSpeed = animSpeed;//0.2f;
	particleGameObject.scale = scale;//1.5f;
}

void ParticleManager::UpdateParticles()
{
	std::vector<int> pVector = Play::CollectGameObjectIDsByType(9);
	Camera* camera = Play::GetCamera();

	for (int pID : pVector)
	{
		GameObject& particleGameObject = Play::GetGameObject(pID);
		camera->DrawOffset(particleGameObject);

		if (Play::IsAnimationComplete(particleGameObject))
		{
			Play::DestroyGameObject(pID);
		}

	}
}

void ParticleManager::DeleteAllParticles()
{
	std::vector<int> pVector = Play::CollectGameObjectIDsByType(9);
	Camera* camera = Play::GetCamera();

	for (int pID : pVector)
	{
		GameObject& particleGameObject = Play::GetGameObject(pID);

		Play::DestroyGameObject(pID);
	}
}
