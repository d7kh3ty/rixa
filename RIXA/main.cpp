#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Camera.h"
#include "Level.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 800;
int DISPLAY_SCALE = 1;

//what is the state of the game
struct GameState
{
	float timer = 0;
	int spriteId = 0;

	// Player attributes
	int speed = 10;
	float angle; // Angle is the speed of bidirectional movement
};

enum GameStateType
{
	menu,
	cutscene,
	play,
	pause,
	gameWin,
	gameLose,
};

//what are the types of game objects
enum GameObjectType
{
	TYPE_NULL = -1,
	angel,
	projectile,
	enemy,
	background,
};

void HandlePlayerControls();
void UpdateCamera();
void UpdateProjectiles();
void UpdateGameObjects();

void DrawOffset(GameObject* go);
void DrawBackground();
bool OutOfBounds(GameObject* go);

GameState gameState;

GameStateType state;

Camera camera(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);

float wBound;
float hBound;

Level level;

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	// Set default game objects
	Play::CreateGameObject(angel, { DISPLAY_WIDTH/2+600,DISPLAY_HEIGHT/2+200 }, 100, "angel");

	level = Level::Level("Data\\Levels\\", "MarsBG", "Data\\Levels\\level1.xml");

	//Play::CreateGameObject(background, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 }, 100, "MarsBG");

	//wBound = 3 / 2 * Play::GetSpriteWidth("MarsBG");
	//hBound = 7 / 4 * Play::GetSpriteHeight("MarsBG");

	wBound = level.getWidth();
	hBound = level.getHeight();

	//approximate directional movement
	gameState.angle = gameState.speed * 0.7;

	//Play::LoadBackground("Data\\Sprites\\MarsBG2.png");
	//does file exist, read file
	//std::ifstream afile = std::ifstream("config.txt");
	//std::getline(afile, message);
	//afile.close();
	
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Delta time
	gameState.timer += elapsedTime;

	// Placeholders of menu and such
	if (state == menu)
	{
		//Play::DrawFontText("132px", "RIXA",
		//	{ DISPLAY_WIDTH / 2, 100 }, Play::CENTRE);
		Play::DrawSprite("MarsBG", { 0,0 }, 0);
		Play::DrawSprite("title", { DISPLAY_WIDTH / 2, 100 }, 0);

		Play::DrawFontText("64px", "PRESS SPACE TO START",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 300 }, Play::CENTRE);

		if(Play::KeyPressed(VK_SPACE))
		{
			state = play;
		}
	}
	else if (state == play)
	{
		HandlePlayerControls();
		level.display(-camera.GetXOffset(), -camera.GetYOffset());
		UpdateGameObjects();
	}

	//draw everything
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	//write to the config file
	//std::ofstream afile = std::ofstream("config.txt");
	//afile << "Hello World";
	//afile.close();

	Play::DestroyManager();
	return PLAY_OK;
}

void HandlePlayerControls()
{
	GameObject& player = Play::GetGameObjectByType(angel);

	player.velocity = { 0, 0 };
	if (Play::KeyDown(0x57)) // W
	{
		player.velocity = { 0, -gameState.speed };
		if (Play::KeyDown(0x41)) // A
			player.velocity = { -gameState.angle, -gameState.angle };
		if (Play::KeyDown(0x44)) // D
			player.velocity = { gameState.angle, -gameState.angle };
	}

	if (Play::KeyDown(0x53)) // S
	{
		player.velocity = { 0, gameState.speed };
		if (Play::KeyDown(0x41)) // A
			player.velocity = { -gameState.angle, gameState.angle };
		if (Play::KeyDown(0x44)) // D
			player.velocity = { gameState.angle, gameState.angle };
	}

	if (Play::KeyDown(0x41)) // A
	{
		player.velocity = { -gameState.speed, 0 };
		if (Play::KeyDown(0x57)) // W
			player.velocity = { -gameState.angle, -gameState.angle };
		if (Play::KeyDown(0x53)) // S
			player.velocity = { -gameState.angle, gameState.angle };
	}
	if (Play::KeyDown(0x44)) // D
	{
		player.velocity = { gameState.speed, 0 };
		if (Play::KeyDown(0x57)) // W
			player.velocity = { gameState.angle, -gameState.angle };
		if (Play::KeyDown(0x53)) // S
			player.velocity = { gameState.angle, gameState.angle };
	}

	// FIRE WEAPON
	if (Play::KeyPressed(VK_LBUTTON)) // Mouse Button
	{
		int p = Play::CreateGameObject(projectile, player.pos, 30, "bullet");
		//Play::GetGameObject(p).velocity = Vector2D( 10, 10 );
		GameObject& nya = Play::GetGameObject(p);
		nya.animSpeed = 0.1f;

		// Find x and y of mouse relative to position
		Point2D mousePos = Play::GetMousePos();
		int x = floor(((mousePos.x + camera.GetXOffset()) - player.pos.x));
		int y = floor(((mousePos.y + camera.GetYOffset()) - player.pos.y));
		std::cout << x << std::endl;
		std::cout << y << std::endl;

		int length = sqrt(x * x + y * y) / 10;
		nya.velocity = Vector2D(x / length, y / length);
	}

	for (auto c : level.getCollisionObjects()) {
		if (c.checkColliding(player.pos.x, player.pos.y, 32)) {
			player.pos = player.oldPos;
		}
	}
	
	// Player bounds checking
	// Issue -- with camera implementation, it may not look like it is leaving display area...
	//if (Play::IsLeavingDisplayArea(player))
	//	if ((player.pos.y > DISPLAY_HEIGHT && player.velocity.y > 0)
	//		|| (player.velocity.y < 0 && player.pos.y < 0))
	//		player.velocity.y = 0;
	//if ((player.pos.x > DISPLAY_WIDTH && player.velocity.x > 0)
	//	|| (player.velocity.x < 0 && player.pos.x < 0))
	//	player.velocity.x = 0;

	// Bounding player
	//if (player.pos.x > )

	player.pos = Point2D(floor(player.pos.x), floor(player.pos.y));
}

void UpdateGameObjects()
{
	UpdateCamera();

	// BACKGROUND MUST BE UPDATED FIRST
	//DrawBackground();

	// Update projectiles
	UpdateProjectiles();

	GameObject& player = Play::GetGameObjectByType(angel);

	// for debug
	for (auto c : level.getCollisionObjects()) {
		PlayGraphics::Instance().DrawRect(c.getTopleft() - camera.GetOffset(), c.getBottomRight() - camera.GetOffset(), {255, 0, 0}, false);
	}

	DrawOffset(&player);

}

void UpdateProjectiles()
{
	std::vector<int> pv = Play::CollectGameObjectIDsByType(projectile);

	for (int id : pv)
	{
		GameObject& p = Play::GetGameObject(id);

		// Destroy out of bounds bullets
		if (OutOfBounds(&p))
		{
			Play::DestroyGameObject(id);
			continue;
		}

		DrawOffset(&p);
	}

}


void UpdateCamera()
{
	GameObject& player = Play::GetGameObjectByType(angel);

	// Camera bounding for level
	if (player.pos.x > 3 / 2 * wBound) // R Bound
	{
		camera.Follow(wBound, player.pos.y);
	}
	else if (player.pos.x < -3 / 2 * wBound) // L Bound
	{
		camera.Follow(-wBound, player.pos.y);
	}
	else if (player.pos.y < -hBound) // Top of the level bound
	{
		camera.Follow(player.pos.x, -hBound);
	}
	else if (player.pos.x < -3 / 2 * wBound && player.pos.y < -hBound)
	{
		camera.Follow(-wBound, -hBound);
	}
	else if (player.pos.x < 3 / 2 * wBound && player.pos.y < -hBound)
	{
		camera.Follow(wBound, -hBound);
	}
	else // Otherwise
	{
		camera.Follow(player.pos.x, player.pos.y);
	}
}


// Draw offset presented by the camera placement
void DrawOffset(GameObject* go)
{
	float oldPosx = go->pos.x;
	float oldPosy = go->pos.y;
	go->pos = { oldPosx - camera.GetXOffset(),oldPosy - camera.GetYOffset() };
	Play::UpdateGameObject(*go);
	Play::DrawObject(*go);
	go->pos = { oldPosx,oldPosy };
	Play::UpdateGameObject(*go);

}

void DrawBackground()
{
	Play::DrawSprite("MarsBG", { -camera.GetXOffset(),0 - camera.GetYOffset() }, 0);
	Play::DrawSprite("MarsBG", { 3 / 2 * Play::GetSpriteWidth("MarsBG") - camera.GetXOffset(),0 - camera.GetYOffset() }, 0);
	Play::DrawSprite("MarsBG", { -3 / 2 * Play::GetSpriteWidth("MarsBG") - camera.GetXOffset(),0 - camera.GetYOffset() }, 0);
	Play::DrawSprite("MarsBG", { 0 - camera.GetXOffset(),-3 / 2 * Play::GetSpriteHeight("MarsBG") - camera.GetYOffset() }, 0);
	Play::DrawSprite("MarsBG", { 3 / 2 * Play::GetSpriteWidth("MarsBG") - camera.GetXOffset(),-3 / 2 * Play::GetSpriteHeight("MarsBG") - camera.GetYOffset() }, 0);
	Play::DrawSprite("MarsBG", { -3 / 2 * Play::GetSpriteWidth("MarsBG") - camera.GetXOffset(),-3 / 2 * Play::GetSpriteHeight("MarsBG") - camera.GetYOffset() }, 0);
}

bool OutOfBounds(GameObject* go)
{
	return abs(go->pos.x) > wBound + 1000 || abs(go->pos.y) > hBound + 1000;
}