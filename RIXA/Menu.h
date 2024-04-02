#pragma once

class Menu
{
public:
	Menu(int displayHeight, int displayWidth, int scrollingSpeed, int flashingInterval);
	void Update(float deltaTime);

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

	float ElapsedTime;
};

