#include "Menu.h"
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

Menu::Menu(int displayHeight, int displayWidth, int scrollingSpeed, int flashingInterval) :
	BackgroundOffset(0),
	BackgroundScrollingSpeed(scrollingSpeed),
	BackgroundHeight(9000),
	DisplayHeight(displayHeight),
	DisplayWidth(displayWidth),
	ElapsedTime(0),
	FlashingInterval(flashingInterval)
{
	
}

void Menu::Update(float deltaTime)
{
	ElapsedTime += deltaTime;

	UpdateBackground(deltaTime);

	UpdateFlashingText();

	DrawControls();
}

void Menu::UpdateBackground(float deltaTime)
{
	// I guess I should replace magic texture strings too at some point
	Play::DrawSprite("MarsBG", { 0, -(BackgroundHeight/2) + DisplayHeight + BackgroundOffset }, 0);

	BackgroundOffset += BackgroundScrollingSpeed * deltaTime;

	if (BackgroundOffset > BackgroundHeight - DisplayHeight)
	{
		BackgroundOffset = 0;
	}
}

void Menu::UpdateFlashingText()
{
	if ((int)ElapsedTime % FlashingInterval == 0)
	{
		// GET RID OF MAGIC NUMBERS AND STRINGS
		Play::DrawFontText("64px", "PRESS SPACE TO START",
			{ DisplayWidth / 2, DisplayHeight - 300 }, Play::CENTRE);
	}
}

void Menu::DrawControls()
{
	Play::DrawSpriteRotated("keyboard_w", { 100, DisplayHeight - 300 + 200 }, 0, 0, 0.45f);
	Play::DrawSpriteRotated("keyboard_a", { 70, DisplayHeight - 270 + 200 }, 0, 0, 0.45f);
	Play::DrawSpriteRotated("keyboard_s", { 100, DisplayHeight - 270 + 200 }, 0, 0, 0.45f);
	Play::DrawSpriteRotated("keyboard_d", { 130, DisplayHeight - 270 + 200 }, 0, 0, 0.45f);
	Play::DrawSpriteRotated("mouse_left", { DisplayWidth - 100, DisplayHeight - 285 + 200 }, 0, 0, 0.6f);

}
