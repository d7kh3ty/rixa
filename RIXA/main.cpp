#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

#include "Camera.h"
#include "Player.h"

#include "TankEnemy.h"
#include "DroneEnemy.h"
#include "TrackedEnemy.h"
#include "TurretEnemy.h"

#include "Level.h"
#include "Menu.h"
#include "Helper.h"

#include "ParticleManager.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include <cmath>

// Screen attributes
const int DISPLAY_WIDTH = 400;
const int DISPLAY_HEIGHT = 400;
const int DISPLAY_SCALE = 2;

// GAME STATES
enum class GameState
{
	menu,
	cutscene,
	play,
	pause,
	gameWin,
	gameLose,
};

// CAMERA AND DRAWING CODE
void InitialiseGame();
void DeconstructGame();
void Update(float deltaTime);

GameState state;

Menu* GameMenu;
Level* level;
Camera* camera;
Player* player;

void Update(float deltaTime)
{
	ProjectileManager::UpdateProjectiles(deltaTime);

	EnemyManager::UpdateEnemies(deltaTime);

	ParticleManager::UpdateParticles();

	// PLAYER DEATH
	if (player->GetIsDead())
	{
		state = GameState::gameLose;
	}

	if (player->GetGameObject().pos.y < -2300)
	{
		state = GameState::gameWin;
	}

  	// for debug
	//for (auto c : level->getCollisionObjects()) {
	//	PlayGraphics::Instance().DrawRect(c.getTopleft() - camera->GetOffset(), c.getBottomRight() - camera->GetOffset(), {255, 0, 0}, false);
	//}
  
}

void InitialiseGame()
{
	camera = new Camera(400, 600, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	Play::SetCamera(camera);

	// Create player and load level
	GameMenu = new Menu(DISPLAY_HEIGHT, DISPLAY_WIDTH, 100, 2);
	player = new Player({ 600,850 });
	Play::SetPlayer(player);
	player->GetGameObject().scale = 0.85f;

	level = new Level("Data\\Levels\\", "desert", "Data\\Levels\\level1.xml");
	Play::SetLevel(level);

	for (auto e : level->getEnemyData()) {
		if (e.type == 1)
			new DroneEnemy({ e.x, e.y });
		if (e.type == 2)
			new TankEnemy({ e.x,e.y });
		if (e.type == 3)
			new TrackedEnemy({ e.x, e.y });
		if (e.type == 4)
			new TurretEnemy({ e.x, e.y });
	}

	state = GameState::menu;

}

void DeconstructGame()
{
	ProjectileManager::DeleteAllProjectiles();
	EnemyManager::DeleteAllEnemies();
	ParticleManager::DeleteAllParticles();
}

// These are the only things that should really stay in the Main.cpp
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	// Done before creating game objects
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	InitialiseGame();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Get player object
	GameObject& playerGameObject = player->GetGameObject();
	PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH }, { 0,DISPLAY_HEIGHT },{255,0,255}, 1);

	// Menu
	if (state == GameState::menu)
	{
		GameMenu->Update(elapsedTime);
		//level->display(-camera->GetXOffset(), -camera->GetYOffset(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
		//Update(elapsedTime);

		// Keep this here
		if (Play::KeyPressed(VK_SPACE))
		{
			delete GameMenu;
			state = GameState::play;
			Play::StartAudioLoop("Data\\Audio\\level_one_shorter.mp3");
		}
	}
	else if (state == GameState::play)
	{

		Play::GetPlayer()->Update(elapsedTime);
		//PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH }, { 0,DISPLAY_HEIGHT },{255,0,255}, 1);
		level->display(-camera->GetXOffset(), -camera->GetYOffset(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
		camera->Update(elapsedTime);
		Update(elapsedTime);

		camera->DrawOffset(&playerGameObject);
		//PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH / 2 }, { 0,DISPLAY_HEIGHT / 2 }, { 255,0,255 }, 0);

	}
	else if (state == GameState::gameLose) {
		ParticleManager::UpdateParticles();

		Play::DrawFontText("64px", "YOU DIED",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 300 }, Play::CENTRE);
		Play::DrawFontText("64px", "PRESS SPACE TO RESTART",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 200 }, Play::CENTRE);


		if (Play::KeyPressed(VK_SPACE))
		{
			Play::StopAudioLoop("Data\\Audio\\level_one_shorter.mp3");
			DeconstructGame();
			InitialiseGame();
		}

	}
	else if (state == GameState::gameWin) {
		ParticleManager::UpdateParticles();

		Play::DrawFontText("64px", "YOU LIVED",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 300 }, Play::CENTRE);
		Play::DrawFontText("64px", "PRESS SPACE TO RESTART",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 200 }, Play::CENTRE);

		if (Play::KeyPressed(VK_SPACE))
		{
			Play::StopAudioLoop("Data\\Audio\\level_one_shorter.mp3");
			DeconstructGame();
			InitialiseGame();
		}

	}


	// Draw
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
