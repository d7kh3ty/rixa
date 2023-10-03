#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Camera.h"

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

//what are the types of game objects
enum GameObjectType
{
	TYPE_NULL = -1,
	angel,
	projectile,
	enemy,
	background,
	shadow,
};

void HandlePlayerControls();
void UpdateGameObjects();
void DrawOffset(GameObject* go);
void DrawBackground();
bool OutOfBounds(GameObject* go);

GameState gameState;

std::string message = "";

Camera camera(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);

float wBound;
float hBound;

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//approximate directional movement
	gameState.angle = gameState.speed * 0.7;
	
	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	// Set default game objects
	Play::CreateGameObject(angel, { DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2 }, 100, "angel");
	Play::CreateGameObject(background, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 }, 100, "MarsBG");

	wBound = 3 / 2 * Play::GetSpriteWidth("MarsBG");
	hBound = 7 / 4 * Play::GetSpriteHeight("MarsBG");
	//Play::LoadBackground("Data\\Sprites\\MarsBG2.png");
	//does file exist, read file
	//std::ifstream afile = std::ifstream("config.txt");
	//std::getline(afile, message);
	//afile.close();

	Play::CreateGameObject(shadow, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 } ,  0, "generic_shadow_one");

	
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Delta time
	gameState.timer += elapsedTime;

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

//what are the directions a gameobject can chose to move in
enum Direction
{
	DIRECTION_NORTH,
	DIRECTION_NORTH_EAST,
	DIRECTION_EAST,
	DIRECTION_SOUTH_EAST,
	DIRECTION_SOUTH,
	DIRECTION_SOUTH_WEST,
	DIRECTION_WEST,
	DIRECTION_NORTH_WEST,
	IDLE,
};



void HandlePlayerControls()
{
	GameObject& player = Play::GetGameObjectByType(angel);

	Direction direction = IDLE;

	if (Play::KeyDown(0x57)) { // W
		direction = DIRECTION_NORTH;
	}
	if (Play::KeyDown(0x44)) { // D
		direction = DIRECTION_EAST;
	}
	if (Play::KeyDown(0x53)) { // S
		direction = DIRECTION_SOUTH;
	}

	if (Play::KeyDown(0x41)) { // A
		direction = DIRECTION_WEST;
	}
	if (Play::KeyDown(0x57) && Play::KeyDown(0x44)) { // W & D
		direction = DIRECTION_NORTH_EAST;
	}
	if (Play::KeyDown(0x53) && Play::KeyDown(0x44)) { // S & D
		direction = DIRECTION_SOUTH_EAST;
	}
	if (Play::KeyDown(0x53) && Play::KeyDown(0x41)) // A & S
	{
		direction = DIRECTION_SOUTH_WEST;
	}
	if (Play::KeyDown(0x57) && Play::KeyDown(0x41)) // W & A
	{	
		direction = DIRECTION_NORTH_WEST;
	}



	switch (direction) {
	case IDLE:
		Play::SetSprite(player, "angel_walk_north", 0.0f);
		player.velocity = { 0, 0 };
		break;
	
	case DIRECTION_NORTH:
		Play::SetSprite(player, "angel_walk_north", 0.07f);
		player.velocity = { 0, -gameState.speed };
		break;
	case DIRECTION_NORTH_EAST:
		Play::SetSprite(player, "angel_walk_northeast", 0.07f);
		player.velocity = { gameState.angle, -gameState.angle };
		break;
	case DIRECTION_EAST:
		Play::SetSprite(player, "angel_walk_east", 0.07f);
		player.velocity = { gameState.speed, 0 };
		break;
	case DIRECTION_SOUTH_EAST:
		Play::SetSprite(player, "angel_walk_southeast", 0.07f);
		player.velocity = { gameState.angle, gameState.angle };
		break;
	case DIRECTION_SOUTH:
		Play::SetSprite(player, "angel_walk_south", 0.07f);
		player.velocity = { 0, gameState.speed };
		break;
	case DIRECTION_SOUTH_WEST:
		Play::SetSprite(player, "angel_walk_southwest", 0.07f);
		player.velocity = { -gameState.angle, gameState.angle };
		break;
	case DIRECTION_WEST:
		Play::SetSprite(player, "angel_walk_west", 0.07f);
		player.velocity = { -gameState.angle, 0 };
		break;
	case DIRECTION_NORTH_WEST:
		Play::SetSprite(player, "angel_walk_northwest", 0.07f);
		player.velocity = { -gameState.angle, -gameState.angle };
		break;
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
	GameObject& player = Play::GetGameObjectByType(angel);
	GameObject& shadowGO = Play::GetGameObjectByType(shadow);


	// Camera bounding for level
	if(player.pos.x > 3/2 * wBound) // R Bound
	{
		camera.Follow(wBound, player.pos.y);
	}
	else if(player.pos.x < - 3 / 2 * wBound) // L Bound
	{
		camera.Follow(-wBound, player.pos.y);
	}
	else if(player.pos.y < - hBound) // Top of the level bound
	{
		camera.Follow(player.pos.x, - hBound);
	}
	else // Otherwise
	{
		camera.Follow(player.pos.x, player.pos.y);
	}

	// BACKGROUND MUST BE UPDATED FIRST
	DrawBackground();

	// Update projectiles
	std::vector<int> pv = Play::CollectGameObjectIDsByType(projectile);

	for (int id : pv)
	{
		GameObject& p = Play::GetGameObject(id);

		// Destroy out of bounds bullets
		if(OutOfBounds(&p))
		{
			Play::DestroyGameObject(id);
			continue;
		}

		DrawOffset(&p);
	}
	shadowGO.pos.x = player.pos.x - 30;
	shadowGO.pos.y = player.pos.y + 50;
	DrawOffset(&shadowGO);
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