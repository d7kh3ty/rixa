#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 720;
int DISPLAY_SCALE = 1;

std::string message = "";

//what is the state of the game
struct GameState
{
	float timer = 0;
	int spriteId = 0;
	int speed = 10;
	float angle;
};


GameState gameState;
//what are the types of game objects
enum GameObjectType
{
	TYPE_NULL = -1,
	angel7,
	projectile,
	enemy
};

void HandlePlayerControls()
{
	GameObject& player = Play::GetGameObjectByType( angel7 );

	player.velocity = { 0, 0 };
	if( Play::KeyDown( 0x57 ) ) // W
	{
		player.velocity = { 0, -gameState.speed };
		if (Play::KeyDown(0x41)) // A
			player.velocity = { -gameState.angle, -gameState.angle };
		if (Play::KeyDown(0x44)) // D
			player.velocity = { gameState.angle, -gameState.angle };
	}

	if( Play::KeyDown( 0x53 ) ) // S
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
		if( Play::KeyDown( 0x53 ) ) // S
			player.velocity = { -gameState.angle, gameState.angle };
	}
	if( Play::KeyDown( 0x44 ) ) // D
	{
		player.velocity = { gameState.speed, 0 };
		if (Play::KeyDown(0x57)) // W
			player.velocity = { gameState.angle, -gameState.angle };
		if( Play::KeyDown( 0x53 ) ) // S
			player.velocity = { gameState.angle, gameState.angle };
	}

	// FIRE WEAPON
	if (Play::KeyPressed(0x45)) // E
	{
		auto p = Play::CreateGameObject( projectile, player.pos, 30, "star");
		//Play::GetGameObject(p).velocity = Vector2D( 10, 10 );
		GameObject& nya = Play::GetGameObject(p);
		Point2D mousePos = Play::GetMousePos();
		int x = floor((mousePos.x - player.pos.x));
		int y = floor((mousePos.y - player.pos.y));
		std::cout << x << std::endl;
		std::cout << y << std::endl;
		int length = sqrt(x * x + y * y)/10;
		nya.velocity = Vector2D(x/length, y/length);
	}

	if(Play::IsLeavingDisplayArea(player))
		if ((player.pos.y > DISPLAY_HEIGHT && player.velocity.y > 0) 
			|| (player.velocity.y < 0 && player.pos.y < 0))
			player.velocity.y = 0;
		if ((player.pos.x > DISPLAY_WIDTH && player.velocity.x > 0) 
			|| (player.velocity.x < 0 && player.pos.x < 0))
			player.velocity.x = 0;

	Play::UpdateGameObject( player );
}

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//approximate directional movement
	gameState.angle = gameState.speed * 0.7;
	
	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	//game objects created
	Play::CreateGameObject(angel7, { 641,600 }, 641, "angel");

	//does file exist, read file
	//std::ifstream afile = std::ifstream("config.txt");
	//std::getline(afile, message);
	//afile.close();
	
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	gameState.timer += elapsedTime;

	// background
	Play::Colour c = {0,30,90};
	Play::ClearDrawingBuffer(c);
	
	//find game object by type
	GameObject& player = Play::GetGameObjectByType(angel7);

	//set the players sprite
	Play::SetSprite(player, "angel",0);

	//make sure it's up to date
	Play::UpdateGameObject(player);
	HandlePlayerControls();
	auto pv = Play::CollectGameObjectIDsByType(projectile);
	for (int id : pv)
	{
		GameObject& p = Play::GetGameObject(id);
		//nya.velocity = Vector2D(1.5f, 1.5f);
		Play::SetSprite(p, "star",0.1f);
		Play::DrawObject(p);
		Play::UpdateGameObject(p);
	}
	Play::DrawObject(player);

	// reset floats to ints for perfect pixel graphics
	player.pos = Point2D(floor(player.pos.x), floor(player.pos.y));

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