#include "LoseState.h"
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "../Manager/ParticleManager.h"

void LoseState::Update(float deltaTime)
{
	ParticleManager::UpdateParticles();

	Play::DrawFontText("64px", "YOU DID NOT SURVIVE",
		{ 400 / 2, 400 - 300 }, Play::CENTRE);
	Play::DrawFontText("64px", "PRESS SPACE TO RESTART",
		{ 400 / 2, 400 - 100 }, Play::CENTRE);

	if (Play::KeyPressed(VK_SPACE))
	{
		Play::StopAudioLoop("Data\\Audio\\level_one_shorter.mp3");
		Play::SetGameState(GameState::RESTART);
	}
}
