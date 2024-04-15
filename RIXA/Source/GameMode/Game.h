#pragma once
#include "GameMode.h"

class Camera;
class Player;
class Level;

// Game modes need to make sure to deallocate memory in their constructor.
class Game : public GameMode
{
public:
	Game();
	~Game();
	void Update(float deltaTime) override;

private:
	Camera* GameCamera;
	Player* GamePlayer;
	Level* GameLevel;
};

