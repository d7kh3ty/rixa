#include "Helper.h"

float Helper::Distance(const GameObject* gameObject1, const GameObject* gameObject2)
{
	Point2D pos1 = gameObject1->pos;
	Point2D pos2 = gameObject2->pos;

	float x = pos2.x - pos1.x;
	float y = pos2.y - pos1.y;

	float distance = sqrt(x * x + y * y);

	return distance;
}

bool Helper::OutOfBounds(const GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return false;
	}

	Point2D pos = gameObject->pos;
	bool outOfBounds = abs(pos.x) > WidthBound + 1000 || abs(pos.y) > HeightBound + 1000;
	return OutOfBounds;
}
