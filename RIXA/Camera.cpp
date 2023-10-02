#include "Camera.h"

Camera::Camera(float xOffset, float yOffset, int width, int height)
{
	xOffset_ = xOffset;
	yOffset_ = yOffset;
	height_ = height;
	width_ = width;
}

float Camera::GetXOffset()
{
	return xOffset_;
}

float Camera::GetYOffset()
{
	return yOffset_;
}

void Camera::Move(float x, float y)
{
	xOffset_ += x;
	yOffset_ += y;
}

void Camera::Follow(float x, float y)
{
	xOffset_ = x - width_ / 2;
	yOffset_ = y - height_ / 2;
}
