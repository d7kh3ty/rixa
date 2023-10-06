#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Camera.h"
#include "Level.h"
#include <cmath>

int DISPLAY_WIDTH = 400;
int DISPLAY_HEIGHT = 400;
int DISPLAY_SCALE = 2;

// Game states
enum GameStateType
{
	menu,
	cutscene,
	play,
	pause,
	gameWin,
	gameLose,
};

GameStateType state;

// Game object types
enum GameObjectType
{
	TYPE_NULL = -1,
	angel,
	projectile,
	e_projectile,
	drone,
	tank,
	roller,
	turret,
	background,
	shadow,
	explosion,
};

// Enemy variation types
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

AngelState angelState = STATE_APPEAR;

void Explosion(Point2D pos, const char * exp);
void UpdateExplosion();
void HandlePlayerControls();
void UpdateCamera();
void UpdateProjectiles();
void UpdateGameObjects();

float Distance(Point2D pos1, Point2D pos2);
void DrawOffset(GameObject* go);
void DrawBackground();
bool OutOfBounds(GameObject* go);

Camera camera(0,600,DISPLAY_WIDTH, DISPLAY_HEIGHT);

float wBound;
float hBound;

// top left / bottom right
int tlBound;
int brBound;

Level level;
int offset = 0;

// Player attributes
int playerid;

int playerHealth = 7;
bool playerColour = false;
int playerColourCooldown = 11;
float range = 300.0f;

int playerBulletSpeed = 11;

void updatePlayerColour(Play::Colour c) {
	Play::ColourSprite("angel_walk_north", c);
	Play::ColourSprite("angel_walk_northeast", c);
	Play::ColourSprite("angel_walk_northwest", c);
	Play::ColourSprite("angel_walk_west", c);
	Play::ColourSprite("angel_walk_south", c);
	Play::ColourSprite("angel_walk_southwest", c);
	Play::ColourSprite("angel_walk_southeast", c);
	Play::ColourSprite("angel_walk_east", c);}

// DVD ENEMY CLASS
class Enemy {
public:
	// constructors
	Enemy() {}; 
	Enemy(EnemyType ENEMY_TYPE, Point2f pos, Vector2D vel) {
		// Set sprite, radius and speeds depending on the enemy type given
		if (ENEMY_TYPE == TYPE_ENEMY1)
		{
			type = TYPE_ENEMY1;
			id = Play::CreateGameObject(drone, pos, 15, "cute_south");
			//Play::SetSprite(Play::GetGameObject(id), "cute_south", 0.1f);
			GameObject& go = Play::GetGameObject(id);
			go.animSpeed = 0.1;
			go.scale = 0.5;
		}
		else if(ENEMY_TYPE == TYPE_ENEMY2)
		{
			type = TYPE_ENEMY2;
			id = Play::CreateGameObject(tank, pos, 30, "tank_south");
			GameObject& go = Play::GetGameObject(id);
			go.animSpeed = 0.1;
			go.scale = 2.0;
			speed = 1;
			health = 3;
			detectionRange = 444;
		}
		else if(ENEMY_TYPE == TYPE_ENEMY3)
		{
			type = TYPE_ENEMY3;
			id = Play::CreateGameObject(roller, pos, 10, "tracked_south");
			GameObject& go = Play::GetGameObject(id);
			go.animSpeed = 0.1;
			go.scale = 2.0;
			speed = 5;
			attackSpeed = 11;
			health = 8;
			detectionRange = 666;
		}
		else if (ENEMY_TYPE == TYPE_ENEMY4)
		{
			type = TYPE_ENEMY4;
			id = Play::CreateGameObject(turret, pos, 6, "turret_south");
			GameObject& go = Play::GetGameObject(id);
			go.animSpeed = 0.1;
			go.scale = 2.0;
			speed = 0;
			attackSpeed = 11;
			health = 8;
			detectionRange = 666;
		}

		else
		{
			// If enemy type does not match
			return;
		}

		Play::GetGameObject(id).velocity = vel;

		// for flank position seeking
		if (Play::RandomRoll(2) == 2) {
			leftright = 1;
		}
	}

	// update everything 
	void update() {


		GameObject& enemy = Play::GetGameObject(id);
		enemy.pos.x = floor(enemy.pos.x);
		enemy.pos.y = floor(enemy.pos.y);
		DrawOffset(&enemy);
		if (dead) {
			enemy.velocity = { 0, 0 };
			Play::SetSprite(Play::GetGameObject(id), "cute_northeast", 0.1f);
			Play::ColourSprite("cute_northeast", {255,0,0});
			countdown--;
			if (countdown == 0) {
				Play::DestroyGameObject(id);
				Play::ColourSprite("cute_northeast", {100,100,100});
				//delete this;
			}
			return;
		}
		GameObject& player = Play::GetGameObject(playerid);

		float x = (player.pos.x) - enemy.pos.x;
		float y = (player.pos.y) - enemy.pos.y;
		float length = sqrt(x * x + y * y);

		if (playerDetected) {

			// Collisions in the environment checking
			for (auto c : level.getCollisionObjects()) {
				if (c.checkColliding(enemy.pos.x, enemy.pos.y, 64)) {
					//enemy.pos = enemy.oldPos;
					//player.velocity = player.;
					enemy.velocity = -enemy.velocity;
				}
			}

			// Shoot in direction of player based on attack cooldown
			if (attackCooldown == 0) { //Play::RandomRoll(120) == 1){
				int pid = Play::CreateGameObject(e_projectile, { enemy.pos.x, enemy.pos.y }, 1, "bullet");
				GameObject& bullet = Play::GetGameObject(pid);
				//Play::PlayAudio("tool");
				float speed_check = length / bulletSpeed;
				bullet.velocity = Vector2D(x / speed_check, y / speed_check);
				attackCooldown = attackSpeed;

				float velx = x + Play::RandomRollRange(-300, 300);
				float vely = y + Play::RandomRollRange(-300, 300);
				float magnitude = sqrt(velx * velx + vely * vely) / speed;

				enemy.velocity = { ((velx) / magnitude) , ((vely) / magnitude) };
			} else {
				attackCooldown--;
			}

			// Test implementation
			// Choose a random direction


			//// Move in direction of player to left or right* 10)
			//if (leftright) {
			//	x = floor(((player.pos.x) - enemy.pos.x - 333));
			//}
			//else {
			//	x = floor(((player.pos.x) - enemy.pos.x + 333));
			//}

			//length = sqrt(x * x + y * y);
			//float speed_check = length / speed;

			//if (length > 10) {
			//	enemy.velocity = Vector2D(x / speed_check, y / speed_check);
			//}
			//else {
			//	enemy.velocity = Vector2D(0, 0);
			//}

			if (x != 0) {
				// facing right
				if (x > 0) {
					// facing down
					if (y > 0) {
						dir = DIRECTION_SOUTH_EAST;
					}
					// facing up
					else {
						dir = DIRECTION_NORTH_EAST;
					}
					// facing approximately straight right
					if (y < 20 && y > -20) {
						dir = DIRECTION_EAST;
					}

				} // facing left
				else if (x < 0) {
					// facing down
					if (y > 0) {
						dir = DIRECTION_SOUTH_WEST;
					}
					// facing up
					else {
						dir = DIRECTION_NORTH_WEST;
					}	
					// facing approximately straight left
					if (y < 20 && y > -20) {
						dir = DIRECTION_WEST;
					}
				}	
				// facing approximately straight down
				if (x < 20 && x > -20 && y > 0) {
					dir = DIRECTION_SOUTH;
				}
				// facing approximately straight up
				else if (x < 20 && x > -20 && y < 0) {
					dir = DIRECTION_NORTH;
				}



			} else {
				dir = DIRECTION_SOUTH;	
			}


		}
		else if (length < detectionRange) {
			playerDetected = true; 
		}

		switch (type) {
		case TYPE_ENEMY1:
			switch (dir) {
			case IDLE:
				Play::SetSprite(enemy, "cute_south", 0.1f);
				break;
			case DIRECTION_NORTH:
				Play::SetSprite(enemy, "cute_north", 0.1f);
				break;
			case DIRECTION_NORTH_EAST:
				Play::SetSprite(enemy, "cute_northeast", 0.1f);
				break;
			case DIRECTION_EAST:
				Play::SetSprite(enemy, "cute_east", 0.1f);
				break;
			case DIRECTION_SOUTH_EAST:
				Play::SetSprite(enemy, "cute_southeast", 0.1f);
				break;
			case DIRECTION_SOUTH:
				Play::SetSprite(enemy, "cute_south", 0.1f);
				break;
			case DIRECTION_SOUTH_WEST:
				Play::SetSprite(enemy, "cute_southwest", 0.1f);
				break;
			case DIRECTION_WEST:
				Play::SetSprite(enemy, "cute_west", 0.1f);
				break;
			case DIRECTION_NORTH_WEST:
				Play::SetSprite(enemy, "cute_northwest", 0.1f);
				break;
			}
			break;
		case TYPE_ENEMY2:
			switch (dir) {
			case IDLE:
				Play::SetSprite(enemy, "tank_south", 0.0f);
				break;
			case DIRECTION_NORTH:
				Play::SetSprite(enemy, "tank_north", 0.07f);
				break;
			case DIRECTION_NORTH_EAST:
				Play::SetSprite(enemy, "tank_northeast", 0.07f);
				break;
			case DIRECTION_EAST:
				Play::SetSprite(enemy, "tank_east", 0.07f);
				break;
			case DIRECTION_SOUTH_EAST:
				Play::SetSprite(enemy, "tank_southeast", 0.07f);
				break;
			case DIRECTION_SOUTH:
				Play::SetSprite(enemy, "tank_south", 0.07f);
				break;
			case DIRECTION_SOUTH_WEST:
				Play::SetSprite(enemy, "tank_southwest", 0.07f);
				break;
			case DIRECTION_WEST:
				Play::SetSprite(enemy, "tank_west", 0.07f);
				break;
			case DIRECTION_NORTH_WEST:
				Play::SetSprite(enemy, "tank_northwest", 0.07f);
				break;
			}
			break;
		case TYPE_ENEMY3:
			switch (dir) {
			case IDLE:
				Play::SetSprite(enemy, "tracked_south", 0.0f);
				break;
			case DIRECTION_NORTH:
				Play::SetSprite(enemy, "tracked_north", 0.07f);
				break;
			case DIRECTION_NORTH_EAST:
				Play::SetSprite(enemy, "tracked_northeast", 0.07f);
				break;
			case DIRECTION_EAST:
				Play::SetSprite(enemy, "tracked_east", 0.07f);
				break;
			case DIRECTION_SOUTH_EAST:
				Play::SetSprite(enemy, "tracked_southeast", 0.07f);
				break;
			case DIRECTION_SOUTH:
				Play::SetSprite(enemy, "tracked_south", 0.07f);
				break;
			case DIRECTION_SOUTH_WEST:
				Play::SetSprite(enemy, "tracked_southwest", 0.07f);
				break;
			case DIRECTION_WEST:
				Play::SetSprite(enemy, "tracked_west", 0.07f);
				break;
			case DIRECTION_NORTH_WEST:
				Play::SetSprite(enemy, "tracked_northwest", 0.07f);
				break;
			}

		case TYPE_ENEMY4:
			switch (dir) {
			case IDLE:
				Play::SetSprite(enemy, "turret_south", 0.0f);
				break;
			case DIRECTION_NORTH:
				Play::SetSprite(enemy, "turret_north", 0.07f);
				break;
			case DIRECTION_NORTH_EAST:
				Play::SetSprite(enemy, "turret_northeast", 0.07f);
				break;
			case DIRECTION_EAST:
				Play::SetSprite(enemy, "turret_east", 0.07f);
				break;
			case DIRECTION_SOUTH_EAST:
				Play::SetSprite(enemy, "turret_southeast", 0.07f);
				break;
			case DIRECTION_SOUTH:
				Play::SetSprite(enemy, "turret_south", 0.07f);
				break;
			case DIRECTION_SOUTH_WEST:
				Play::SetSprite(enemy, "turret_southwest", 0.07f);
				break;
			case DIRECTION_WEST:
				Play::SetSprite(enemy, "turret_west", 0.07f);
				break;
			case DIRECTION_NORTH_WEST:
				Play::SetSprite(enemy, "turret_northwest", 0.07f);
				break;
			}
			break;
		}

		// Destroy out of bounds game objects
		if (OutOfBounds(&enemy)) {
			Play::DestroyGameObject(id);
		}
	}

	int getID() {
		return id;
	}

	void kill() {
		dead = true;
	}
	bool isDead() {
		if (countdown <= 0) {
			return true;
		}
	}

	void modHealth(int mod) {
		health += mod;
	}
	int getHealth() {
		return health;
	}

private:
	EnemyType type;
	int id;
	int health = 1;

	int speed = 3;
	int attackSpeed = 44; // lower is faster
	int attackCooldown = 0;

	float bulletSpeed = 3;
	int detectionRange = 222;
	bool playerDetected = false;

	bool leftright = 0; // this is for random flanking

	int countdown = 13;
	bool dead = false;

	Direction dir = DIRECTION_SOUTH;
};

//what is the state of the game
struct GameState
{
	float timer = 0;
	int spriteId = 0;

	// Player attributes
	int speed = 7;
	float angle; // Angle is the speed of bidirectional movement

	vector<Enemy> enemies;
};

GameState gameState;

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
	// Player bounds checking
	// Issue -- with camera implementation, it may not look like it is leaving display area...
	//if (Play::IsLeavingDisplayArea(player))
	//	if ((player.pos.y > DISPLAY_HEIGHT && player.velocity.y > 0)
	//		|| (player.velocity.y < 0 && player.pos.y < 0))
	//		player.velocity.y = 0;
	//if ((player.pos.x > DISPLAY_WIDTH && player.velocity.x > 0)
	//	|| (player.velocity.x < 0 && player.pos.x < 0))
	//	player.velocity.x = 0;


	switch (direction) {
	case IDLE:
		//Play::SetSprite(player, "angel_walk_north", 0.0f);
		player.animSpeed = 0;
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
		player.velocity = { -gameState.speed, 0 };
		break;
	case DIRECTION_NORTH_WEST:
		Play::SetSprite(player, "angel_walk_northwest", 0.07f);
		player.velocity = { -gameState.angle, -gameState.angle };
		break;
	}

	// FIRE WEAPON
	if (Play::KeyPressed(VK_LBUTTON)) // Mouse Button
	{
		//Play::PlayAudio("shot");

		int p = Play::CreateGameObject(projectile, player.pos, 1, "bullet");
		//Play::GetGameObject(p).velocity = Vector2D( 10, 10 );
		GameObject& nya = Play::GetGameObject(p);
		nya.animSpeed = 0.075f;

		// Find x and y of mouse relative to position
		Point2D mousePos = Play::GetMousePos();
		int x = (mousePos.x + camera.GetXOffset()) - player.pos.x;
		int y = (mousePos.y + camera.GetYOffset()) - player.pos.y;

		int length = sqrt(x * x + y * y) / playerBulletSpeed;
		nya.velocity = Vector2D(x / length, y / length);

	}

	// Collisions in the environment checking
	/// replace with level.isColliding()
	for (auto c : level.getCollisionObjects()) {
		if (c.checkColliding(player.pos.x, player.pos.y, player.radius)) {
			player.pos = player.oldPos;
			//player.velocity = player.;
		}
	}

}

void UpdateGameObjects()
{
	
	GameObject& player = Play::GetGameObject(playerid);

	UpdateCamera();

	// BACKGROUND MUST BE UPDATED FIRST
	//DrawBackground();

	// Update projectiles
	UpdateProjectiles();
	UpdateExplosion();

	if (playerHealth <= 0)
	{
		Play::PlayAudio("synthetic_bomb");
		Explosion(player.pos, "exp_bubble");
		Explosion(player.pos + Point2D {20,0}, "exp_pop");
		Explosion(player.pos + Point2D {-20, 0}, "exp_bubble");
		Explosion(player.pos + Point2D {0, 20}, "exp_cloud");
		state = gameLose;
	}

	if (playerColour) {
		playerColourCooldown--;
	}
	if (playerColourCooldown <= 0) {
		playerColour = false;
		playerColourCooldown = 11;
		updatePlayerColour({ 100, 100, 100 });
	}

  	// for debug
	for (auto c : level.getCollisionObjects()) {
		PlayGraphics::Instance().DrawRect(c.getTopleft() - camera.GetOffset(), c.getBottomRight() - camera.GetOffset(), {255, 0, 0}, false);
	}
  
  // Update shadow -- out for some reason?
 //   GameObject& shadowGO = Play::GetGameObjectByType(shadow);
	//shadowGO.pos.x = player.pos.x - 25;
	//shadowGO.pos.y = player.pos.y + 20;
	//DrawOffset(&shadowGO);

	// for optimisation purposes
	///for (int i = 0; i != gameState.enemies.size();) {
	///	if (gameState.enemies[i].isDead()) {
	///		gameState.enemies.erase(gameState.enemies.begin() + i-1);
	///	}
	///	else {
	///		i++;
	///	}
	///}

}

void UpdateProjectiles()
{
	std::vector<int> pv = Play::CollectGameObjectIDsByType(projectile);
	GameObject& player = Play::GetGameObject(playerid);

	// Check is player projectiles hit the enemy
	for (int id : pv)
	{
		bool canDamage = true;
		bool isDestroyed = false;

		GameObject& p = Play::GetGameObject(id);
		//std::vector<int> ev = Play::CollectGameObjectIDsByType(enemy);
		//std::vector<Enemy> ev = gameState.enemies;

		// Both ifs ensures range but allows animation to finish!!!
		if(Distance(player.pos, p.pos) > range)
		{
			//Play::DestroyGameObject(id);
			p.velocity = { 0,0 };
			bool canDamage = false;
		}

		if(!canDamage)
		{
			continue;
		}

		// Destroy out of bounds bullets and animation complete bullets
		// Allows bullets to despawn witthout running expensive enemy checking
		if (OutOfBounds(&p) || Play::IsAnimationComplete(p)) // || level.isColliding(p.pos.x, p.pos.y, p.radius))
		{
			Play::DestroyGameObject(id);
			continue;
		}

		for (auto i = 0; i != gameState.enemies.size();) {
			Enemy enobj = gameState.enemies[i];
			int eid = enobj.getID();
			GameObject& en = Play::GetGameObject(eid);

			if (Play::IsColliding(en, p))
			{
				gameState.enemies[i].modHealth(-1);
				Explosion(en.pos, "exp_pop");
				isDestroyed = true;
				if (gameState.enemies[i].getHealth() <= 0) {
					// This is a feedback hit
					//gameState.enemies.erase(i);
					gameState.enemies[i].kill();

					if (gameState.enemies[i].isDead()) {
						// This is a death hit
						Explosion(en.pos, "exp_bubble");
						Play::PlayAudio("synthetic_bomb");
						gameState.enemies.erase(gameState.enemies.begin() + i);
					}
				}			
				break;
			} else {
				i++;
			}
		}

		if(isDestroyed)
		{
			Play::DestroyGameObject(id);
			continue;
		}

		DrawOffset(&p);
	}

	// We can use the update projectiles to handle this
	std::vector<int> projectiles = Play::CollectGameObjectIDsByType(e_projectile);

	// iterate bullets
	for (int pid : projectiles) {
		GameObject& bullet = Play::GetGameObject(pid);
		// Detect player collision
		if (Play::IsColliding(bullet, player)) {
			updatePlayerColour({ 255, 0, 0 });
			playerColour = true;

			playerHealth--;
			Play::DestroyGameObject(pid);
			//angelState = STATE_DEAD;
			Play::PlayAudio("die");
			break;
		}

		// if want more bullet functions?

		DrawOffset(&bullet);

		if (OutOfBounds(&bullet))//||  level.isColliding(bullet.pos.x, bullet.pos.y, bullet.radius))
		//Play::DestroyGameObject(id);
		{
			Play::DestroyGameObject(pid);
		}
	}

}

void Explosion(Point2D pos, const char * exp)
{
	int eid = Play::CreateGameObject(explosion, pos, 0, exp);
	GameObject& explosion = Play::GetGameObject(eid);
	explosion.animSpeed = 0.2f;
}

void UpdateExplosion()
{
	std::vector<int> expv = Play::CollectGameObjectIDsByType(explosion);

	for (int exid : expv)
	{
		GameObject& explosion = Play::GetGameObject(exid);
		DrawOffset(&explosion);

		if (Play::IsAnimationComplete(explosion))
		{
			Play::DestroyGameObject(exid);
		}

	}
}

void UpdateCamera()
{
	GameObject& player = Play::GetGameObject(playerid);

	// Camera bounding for level
	if (player.pos.x + (DISPLAY_WIDTH / 2) >  wBound) // R Bound
	{
		camera.Follow(wBound - DISPLAY_WIDTH / 2, player.pos.y);
	}
	else if (player.pos.x - DISPLAY_WIDTH / 2 < 0) // L Bound
	{
		camera.Follow(DISPLAY_WIDTH/2, player.pos.y);
	}
	//else if (player.pos.y - DISPLAY_HEIGHT/2  < -hBound) // Top of the level bound
	//{
	//	camera.Follow(player.pos.x, -hBound + DISPLAY_HEIGHT / 2 );
	//}
	//else if (player.pos.x - DISPLAY_WIDTH / 2 < 0 && player.pos.y - DISPLAY_HEIGHT / 2 < -hBound) // Both height and width bounds reached
	//{
	//	camera.Follow(DISPLAY_WIDTH/2, -hBound + DISPLAY_HEIGHT / 2 );
	//}
	//else if (player.pos.x + (DISPLAY_WIDTH / 2) > wBound && player.pos.y - DISPLAY_HEIGHT / 2  < -hBound)
	//{
	//	camera.Follow(wBound, -hBound + DISPLAY_HEIGHT / 2 );
	//}
	else // Otherwise
	{
		camera.Follow(player.pos.x, player.pos.y);
	}
}

float Distance(Point2D pos1, Point2D pos2)
{
	float x = pos2.x - pos1.x;
	float y = pos2.y - pos1.y;

	return sqrt(x*x + y*y);
}

// Draw offset presented by the camera placement
void DrawOffset(GameObject* go)
{
	float oldPosx = go->pos.x;
	float oldPosy = go->pos.y;
	go->pos = { oldPosx - camera.GetXOffset(),oldPosy - camera.GetYOffset() };
	Play::UpdateGameObject(*go);
	Play::DrawObjectRotated(*go);
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

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{

	//must be done before creating game objects	
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	// Set default game objects
	//Play::CreateGameObject(angel, { DISPLAY_WIDTH/2+600,DISPLAY_HEIGHT/2+200 }, 100, "angel");
	playerid = Play::CreateGameObject(angel, { 66,900 }, 33, "angel_walk_north");
	level = Level::Level("Data\\Levels\\", "MarsBG", "Data\\Levels\\level1.xml");

	//Play::CreateGameObject(background, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 }, 100, "MarsBG");

	//wBound = 3 / 2 * Play::GetSpriteWidth("MarsBG");
	//hBound = 7 / 4 * Play::GetSpriteHeight("MarsBG");

	wBound = 32 * 32;//level.getWidth();
	hBound = 128 * 32 -100;//level.getHeight();

	//approximate directional movement
	gameState.angle = gameState.speed * 0.7;

	Play::CreateGameObject(shadow, { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2 } ,  0, "generic_shadow_one");

	//gameState.enemies.push_back(Enemy(TYPE_ENEMY2, {500, 600}, {0,0}));
	for (auto e : level.getEnemyData()) {
		if (e.type == 1)
			gameState.enemies.push_back(Enemy(TYPE_ENEMY1, {e.x, e.y}, {0,0}));
		if (e.type == 2)
			gameState.enemies.push_back(Enemy(TYPE_ENEMY2, {e.x, e.y}, {0,0}));
		if (e.type ==3)
			gameState.enemies.push_back(Enemy(TYPE_ENEMY3, {e.x, e.y}, {0,0}));
	}
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Delta time
	gameState.timer += elapsedTime;

	// Get player object
	GameObject& player = Play::GetGameObject(playerid);
	PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH }, { 0,DISPLAY_HEIGHT },{255,0,255}, 1);

	// Placeholders of menu and such
	if (state == menu)
	{
		
		Play::DrawSprite("MarsBG", { 0, -4500 + DISPLAY_HEIGHT + offset}, 0);
		Play::DrawSprite("title", { DISPLAY_WIDTH / 2, 300 }, 0);

		offset+=8;//+=2250/128;

		if(offset > 9000 - DISPLAY_HEIGHT)
		{
			offset = 0;
		}

		if((int)gameState.timer % 2 == 0)
		{
			Play::DrawFontText("64px", "PRESS SPACE TO START",
				{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 300 }, Play::CENTRE);

		}

		if (Play::KeyPressed(VK_SPACE))
		{
			state = play;
			Play::StartAudioLoop("Data\\Audio\\level_one_shorter.mp3");
		}
	}
	else if (state == play)
	{
		HandlePlayerControls();
		PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH }, { 0,DISPLAY_HEIGHT },{255,0,255}, 1);
		level.display(-camera.GetXOffset(), -camera.GetYOffset(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
		UpdateGameObjects();

		// Testing for enemy generation
		for (auto i = 0; i != gameState.enemies.size(); i++)
		{
			gameState.enemies[i].update();
		}
		DrawOffset(&player);
		PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH / 2 }, { 0,DISPLAY_HEIGHT / 2 }, { 255,0,255 }, 0);

	}
	else if (state == gameLose) {

		//PlayGraphics::Instance().DrawRect({ 0,DISPLAY_WIDTH }, { 0,DISPLAY_HEIGHT }, { 255,0,255 }, 1);
		//level.display(-camera.GetXOffset(), -camera.GetYOffset(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
		UpdateExplosion();

		Play::DrawFontText("64px", "YOU HAVE FAILED D:",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 300 }, Play::CENTRE);

		Play::DrawFontText("64px", "PRESS SPACE TO TRY AGAIN",
			{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 100 }, Play::CENTRE);


		if (Play::KeyPressed(VK_SPACE))
		{
			state = play;
			MainGameEntry(0, 0);
		}

	}

	//draw everything
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
