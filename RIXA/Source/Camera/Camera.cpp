#include "Camera.h"
#include "../Helper.h"
#include "../Actor/Player.h"

#include <cmath>

Camera::Camera(float xOffset, float yOffset, int width, int height) :
	XOffset(xOffset),
	YOffset(yOffset),
	DISPLAY_WIDTH(width),
	DISPLAY_HEIGHT(height),
	LerpAlpha(4.0f)
{
}

// Draw object with respect to camera offsetes
void Camera::DrawOffset(GameObject& gameObject) const
{
	// Get positions
	float oldPosx = gameObject.pos.x;
	float oldPosy = gameObject.pos.y;

	// Translate according to offset and draw
	gameObject.pos = { oldPosx - GetXOffset(),oldPosy - GetYOffset() };
	Play::UpdateGameObject(gameObject);
	Play::DrawObjectRotated(gameObject);

	// Then set it back
	gameObject.pos = { oldPosx,oldPosy };
	Play::UpdateGameObject(gameObject);
}

void Camera::Update(float deltaTime)
{
	GameObject& playerGameObject = Play::GetPlayer()->GetGameObject();

	// Camera bounding for level
	if (playerGameObject.pos.x + (DISPLAY_WIDTH / 2) > Helper::WidthBound) // R Bound
	{
		Follow(deltaTime, Helper::WidthBound - DISPLAY_WIDTH / 2, playerGameObject.pos.y);
	}
	else if (playerGameObject.pos.x - DISPLAY_WIDTH / 2 < 0) // L Bound
	{
		Follow(deltaTime, DISPLAY_WIDTH / 2, playerGameObject.pos.y);
	}
	else // Otherwise
	{
		Follow(deltaTime, playerGameObject.pos.x, playerGameObject.pos.y);
	}

}

void Camera::Follow(float deltaTime, float x, float y)
{
	// Centre x offset
	XOffset = std::lerp(XOffset, x - (DISPLAY_WIDTH / 2), LerpAlpha * deltaTime);

	// Bounds the camera from the bottom
	if (y - (DISPLAY_HEIGHT / 2) < YOffset)
	{
		YOffset = std::lerp(YOffset, y - (DISPLAY_HEIGHT / 2), LerpAlpha *deltaTime);
	}
}
