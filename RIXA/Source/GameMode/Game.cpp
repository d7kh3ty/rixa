#include "Game.h"
#include "../Manager/ParticleManager.h"
#include "../Manager/EnemyManager.h"
#include "../Manager/ProjectileManager.h"

#include "../Level/Level.h"
#include "../GameMode/Menu.h"
#include "../Camera/Camera.h"

#include "../Actor/Player.h"

#include "../Actor/TankEnemy.h"
#include "../Actor/DroneEnemy.h"
#include "../Actor/TrackedEnemy.h"
#include "../Actor/TurretEnemy.h"

Game::Game()
{
	GameCamera = new Camera(400, 600, 400, 400);
	Play::SetCamera(GameCamera);

	// Create player and load level
	GamePlayer = new Player({ 600,850 });
	Play::SetPlayer(GamePlayer);
	GamePlayer->GetGameObject().scale = 0.85f;

	GameLevel = new Level("Data\\Levels\\", "desert", "Data\\Levels\\level1.xml");
	Play::SetLevel(GameLevel);

	for (auto e : GameLevel->getEnemyData()) {
		if (e.type == 1)
			new DroneEnemy({ e.x, e.y });
		if (e.type == 2)
			new TankEnemy({ e.x,e.y });
		if (e.type == 3)
			new TrackedEnemy({ e.x, e.y });
		if (e.type == 4)
			new TurretEnemy({ e.x, e.y });
	}
}

// Delete everything game related -- this should only happen when exiting or restarting
Game::~Game()
{
	delete GamePlayer;
	delete GameCamera;
	delete GameLevel;

	ProjectileManager::DeleteAllProjectiles();
	EnemyManager::DeleteAllEnemies();
	ParticleManager::DeleteAllParticles();
}

void Game::Update(float deltaTime)
{
	GamePlayer->Update(deltaTime);
	GameLevel->display(-GameCamera->GetXOffset(), -GameCamera->GetYOffset(), 400, 400);
	GameCamera->Update(deltaTime);

	ProjectileManager::UpdateProjectiles(deltaTime);

	EnemyManager::UpdateEnemies(deltaTime);

	ParticleManager::UpdateParticles();

	if (GamePlayer->GetIsDead())
	{
		Play::SetGameState(GameState::gameLose);
		return;
	}

	if (GamePlayer->GetGameObject().pos.y < -2300)
	{
		Play::SetGameState(GameState::gameWin);
		return;
	}

	GameCamera->DrawOffset(GamePlayer->GetGameObject());
}
