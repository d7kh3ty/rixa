#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Camera.h"
#include "Level.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 800;
int DISPLAY_SCALE = 1;
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
	e_projectile,
	enemy,
	background,
};

enum EnemyType
{
	// THIS MUST BE DONE
	TYPE_ENEMY1=101,
	TYPE_ENEMY2=102,
	TYPE_ENEMY3=103,
	TYPE_ENEMY4=104,
};
// Player states
enum AngelState
{
	STATE_APPEAR = 0,
	STATE_HALT,
	STATE_PLAY,
	STATE_DEAD,
};

AngelState angelState = STATE_APPEAR;

void HandlePlayerControls();
void UpdateCamera();
void UpdateProjectiles();
void UpdateGameObjects();

void DrawOffset(GameObject* go);
void DrawBackground();
bool OutOfBounds(GameObject* go);


Camera camera(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

float wBound;
float hBound;

Level level;

int playerid;


// DVD ENEMY CLASS
class Enemy {
public:
	// constructors
	Enemy() {}; 
	Enemy(EnemyType ENEMY_TYPE, Point2f pos, Vector2D vel) {
		// Set sprite, radius and speeds depending on the enemy type given
		if (ENEMY_TYPE == TYPE_ENEMY1)
		{
			id = Play::CreateGameObject(enemy, pos, 10, "coin");
			Play::GetGameObject(id).animSpeed = 1;
		}
		else
		{
			// If enemy type does not match
			return;
		}

		Play::GetGameObject(id).velocity = vel;
	}

	// update everything 
	void update() {

		GameObject& player = Play::GetGameObject(playerid);
		GameObject& enemy = Play::GetGameObject(id);
		//Play::SetSprite(enemy, "coins_2", 0.25f); //
		int x = floor(((player.pos.x + camera.GetXOffset()) - enemy.pos.x));
		int y = floor(((player.pos.y + camera.GetYOffset()) - enemy.pos.y));
		float length = sqrt(x * x + y * y);

		// Shoot in direction of player
		if (Play::RandomRoll(100) > 99) {
			int pid = Play::CreateGameObject(e_projectile, { enemy.pos.x, enemy.pos.y }, 90, "bullet");
			GameObject& bullet = Play::GetGameObject(pid);
			Play::PlayAudio("tool");
			float speed_check = length / bulletSpeed;
			bullet.velocity = Vector2D(x / speed_check, y / speed_check);
		}

		DrawOffset(&enemy);

		// Destroy out of bounds game objects
		if (OutOfBounds(&enemy)) {
			Play::DestroyGameObject(id);
		}

		// We can use the update projectiles to handle this
		std::vector<int> projectiles = Play::CollectGameObjectIDsByType(e_projectile);

		for (int pid : projectiles) {
			GameObject& bullet = Play::GetGameObject(pid);
			if (angelState != STATE_DEAD && Play::IsColliding(bullet, player)) {
				angelState = STATE_DEAD;
				Play::PlayAudio("die");
			}

			// if want more bullet functions?

			DrawOffset(&bullet);

			if (OutOfBounds(&bullet))
			{
				Play::DestroyGameObject(pid);
			}
		}

		if (length < detectionRange) {
			playerDetected = true;
		}
	}

private:
	EnemyType type;
	int id;
	int bulletSpeed = 5;
	int detectionRange = 10;
	bool playerDetected = false;
};

//what is the state of the game
struct GameState
{
	float timer = 0;
	int spriteId = 0;

	// Player attributes
	int speed = 10;
	float angle; // Angle is the speed of bidirectional movement

	vector<Enemy> enemies;
};

GameStateType state;

GameState gameState;

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	// Set default game objects
	playerid = Play::CreateGameObject(angel, { DISPLAY_WIDTH/2+600,DISPLAY_HEIGHT/2+200 }, 100, "angel");

	level = Level::Level("Data\\Levels\\", "island", "Data\\Levels\\test_map.xml");

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

	//Play::CreateGameObject(shadow, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 } ,  0, "generic_shadow_one");

	// DVD: enemies 
	gameState.enemies.push_back(Enemy(TYPE_ENEMY1, {500, 500}, {10,10}));

	gameState.enemies.push_back(Enemy(TYPE_ENEMY1, { 600, 500 }, { -10,10 }));

	gameState.enemies.push_back(Enemy(TYPE_ENEMY1, { 500, 600 }, { 10,-10 }));

}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Delta time
	gameState.timer += elapsedTime;

	// Get player object
	GameObject& player = Play::GetGameObject(playerid);

	// Placeholders of menu and such
	if (state == menu)
	{
		//Play::DrawFontText("132px", "RIXA",
		//	{ DISPLAY_WIDTH / 2, 100 }, Play::CENTRE);
		Play::DrawSprite("MarsBG", { 0,0 }, 0);
		Play::DrawSprite("title", { DISPLAY_WIDTH / 2, 300 }, 0);

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
		// DVD
		//"Any similarity with fictitious events or characters was purely coincidental."
		//v_cershinsky.UpdateEnemy();
		//v_cershinsky.UpdateEnemyProjectiles();

		//j_bidet.UpdateEnemy();
		//j_bidet.UpdateEnemyProjectiles();

		//ursula_L.UpdateEnemy();
		//ursula_L.UpdateEnemyProjectiles();
		for (auto enemy : gameState.enemies)
		{
			enemy.update();
		}
		DrawOffset(&player);
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
	GameObject& player = Play::GetGameObject(playerid);

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
}


void UpdateGameObjects()
{
	UpdateCamera();

	// BACKGROUND MUST BE UPDATED FIRST
	//DrawBackground();

	// Update projectiles
	UpdateProjectiles();

  	// for debug
	for (auto c : level.getCollisionObjects()) {
		PlayGraphics::Instance().DrawRect(c.getTopleft() - camera.GetOffset(), c.getBottomRight() - camera.GetOffset(), {255, 0, 0}, false);
	}
  
  // Update player and shadow
	GameObject& player = Play::GetGameObject(playerid);
    //GameObject& shadowGO = Play::GetGameObjectByType(shadow);
	//shadowGO.pos.x = player.pos.x - 30;
	//shadowGO.pos.y = player.pos.y + 50;
	//DrawOffset(&shadowGO);


	// for debug
	for (auto c : level.getCollisionObjects()) {
		PlayGraphics::Instance().DrawRect(c.getTopleft() - camera.GetOffset(), c.getBottomRight() - camera.GetOffset(), {255, 0, 0}, false);
	}


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
	GameObject& player = Play::GetGameObject(playerid);

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