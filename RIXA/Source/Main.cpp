#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

#include "GameMode/Menu.h"
#include "GameMode/Game.h"
#include "GameMode/WinState.h"
#include "GameMode/LoseState.h"

// Screen attributes
const int DISPLAY_WIDTH = 400;
const int DISPLAY_HEIGHT = 400;
const int DISPLAY_SCALE = 2;

void InitialiseGame();
void DeconstructGame();
void RestartGame();

// Game modes
Menu* menu;
Game* game;
WinState* winState;
LoseState* loseState;

// Initialise game mode objects
void InitialiseGame()
{
	menu = new Menu(DISPLAY_HEIGHT, DISPLAY_WIDTH, 100, 2);
	game = new Game;
	winState = new WinState;
	loseState = new LoseState;

	Play::SetGameState(GameState::menu);
}

// Free memory when closing the game
void DeconstructGame()
{
	delete menu;
	delete game;
	delete winState;
	delete loseState;
}

// Restart game by creating a new instance and deleting the old
void RestartGame()
{
	delete game;
	game = new Game();

	Play::SetGameState(GameState::menu);
}

// Entry and initialisation
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
	// Game state is stored in Play.h script, allows gamemodes to access and change it
	switch (Play::GetGameState()) 
	{
		case GameState::menu:
			menu->Update(elapsedTime);
			break;

		case GameState::play:
			game->Update(elapsedTime);
			break;

		case GameState::gameLose:
			loseState->Update(elapsedTime);
			break;

		case GameState::gameWin:
			winState->Update(elapsedTime);
			break;

		case GameState::RESTART:
			RestartGame();
			break;
		}


	// Draw
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	DeconstructGame();
	Play::DestroyManager();
	return PLAY_OK;
}
