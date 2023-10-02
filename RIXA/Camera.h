#pragma once
//#include <iostream>

class Camera
{
public:
	Camera(float xOffset, float yOffset, int width, int height);
	float GetXOffset();
	float GetYOffset();
	void Move(float x, float y);
	void Follow(float x, float y);
private:
	float xOffset_;
	float yOffset_;
	int width_;
	int height_;
};

