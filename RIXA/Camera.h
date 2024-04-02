#pragma once
//#include <iostream>
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

class Camera
{
public:
	Camera(float xOffset, float yOffset, int width, int height);
	void DrawOffset(GameObject* gameObject);

	void Update(float deltaTime);

	float GetXOffset();
	float GetYOffset();
	Point2f GetOffset();
	void Follow(float deltaTime, float x, float y);

private:
	float XOffset;
	float YOffset;
	const int DISPLAY_WIDTH;
	const int DISPLAY_HEIGHT;

	const float LerpAlpha;
};

