#pragma once
#include "GameMode.h"

class Menu : public GameMode
{
public:
	Menu(int displayHeight, int displayWidth, int scrollingSpeed, int flashingInterval);
	void Update(float deltaTime) override;

private:
	void UpdateBackground(float deltaTime);
	void UpdateFlashingText();
	void DrawControls();

	int BackgroundOffset;
	const int BackgroundScrollingSpeed;
	const int BackgroundHeight;

	const int DisplayHeight;
	const int DisplayWidth;

	int FlashingInterval;
};

