#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

class Helper
{
public:
	static float Distance(const GameObject* gameObject1, const GameObject* gameObject2);
	static bool OutOfBounds(const GameObject* gameObject);

	static constexpr float WidthBound = (32 * 32);
	static constexpr float HeightBound = ((128 * 32) - 100);
};

