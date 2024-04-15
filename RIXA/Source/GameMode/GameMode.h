#pragma once
class GameMode
{
public:
	GameMode();
	virtual void Update(float deltaTime);

protected:
	float ElapsedTime;
};

