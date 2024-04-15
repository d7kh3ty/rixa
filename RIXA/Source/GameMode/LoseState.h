#pragma once
#include "GameMode.h"

class LoseState : public GameMode
{
public:
	void Update(float deltaTime) override;
};

