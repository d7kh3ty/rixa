#pragma once
#include "Play.h"

class CollisionBox {
public:
	CollisionBox(Point2f top_left, Point2f bottom_right);
	bool checkColliding(int x2, int y2, int radius);
	Point2f getTopleft();
	Point2f getBottomRight();
private:
	Point2f top_left_;
	Point2f bottom_right_;

};

