#pragma once
//#include <iostream>
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

class Camera
{
public:
	Camera(float xOffset, float yOffset, int width, int height);
	void DrawOffset(GameObject& gameObject) const;

	void Update(float deltaTime);

	float GetXOffset() const { return XOffset; }
	float GetYOffset() const { return YOffset; }
	Point2f GetOffset() const { return Point2f(XOffset, YOffset); }
	void Follow(float deltaTime, float x, float y);

private:
	float XOffset;
	float YOffset;
	const int DISPLAY_WIDTH;
	const int DISPLAY_HEIGHT;

	const float LerpAlpha;
};

