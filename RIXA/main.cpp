#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Camera.h"
#include "Level.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 720;
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
void UpdateGameObjects();
void DrawOffset(GameObject* go);

GameState gameState;
std::string message = "";
Camera camera(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
Level level;

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//approximate directional movement
	gameState.angle = gameState.speed * 0.7;
	
	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	// Set default game objects
	Play::CreateGameObject(angel, { DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2 }, 100, "angel");
	//Play::CreateGameObject(background, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 }, 100, "MarsBG");

	level = Level::Level("Data\\Levels\\", "island", "Data\\Levels\\test_map.xml");

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

	level.display(-camera.GetXOffset(), -camera.GetYOffset());

	HandlePlayerControls();
	UpdateGameObjects();

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
	if (Play::KeyPressed(0x45)) // E
	{
		int p = Play::CreateGameObject(projectile, player.pos, 30, "star");
		//Play::GetGameObject(p).velocity = Vector2D( 10, 10 );
		GameObject& nya = Play::GetGameObject(p);

		// Find x and y of mouse relative to position
		Point2D mousePos = Play::GetMousePos();
		int x = floor((mousePos.x - player.pos.x));
		int y = floor((mousePos.y - player.pos.y));
		std::cout << x << std::endl;
		std::cout << y << std::endl;

		int length = sqrt(x * x + y * y) / 10;
		nya.velocity = Vector2D(x / length, y / length);
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

	player.pos = Point2D(floor(player.pos.x), floor(player.pos.y));
}

void UpdateGameObjects()
{
	// Camera must state all the offsets required to draw each sprite
	// Fixed offset
		// playeroff = player.pos - offset pos
	// if is not
	GameObject& player = Play::GetGameObjectByType(angel);

	camera.Follow(player.pos.x, player.pos.y);

	// BACKGROUND MUST BE UPDATED FIRST
	GameObject& bg = Play::GetGameObjectByType(background);
	//DrawOffset(&bg);

	// Update projectiles
	std::vector<int> pv = Play::CollectGameObjectIDsByType(projectile);

	for (int id : pv)
	{
		GameObject& p = Play::GetGameObject(id);
		DrawOffset(&p);
	}

	//Update player
	//DrawOffset(&player);
	//GameObject& player = Play::GetGameObjectByType(angel);
	//Play::UpdateGameObject(player);
	//Play::DrawObject(player);
	DrawOffset(&player);

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
