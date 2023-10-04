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

	if(y - height_ / 2 < yOffset_)
	{
		yOffset_ = y - height_ / 2;
	}
	//std::cout << "xOff: " << xOffset_ << "\n";
	//std::cout << "yOff: " << yOffset_ << "\n";
}

Point2f Camera::GetOffset() {
	return Point2f(xOffset_, yOffset_);
}
