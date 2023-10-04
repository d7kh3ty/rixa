#include "Collision.h"

CollisionBox::CollisionBox(Point2f top_left, Point2f bottom_right) {
	top_left_ = top_left;
	bottom_right_ = bottom_right;
}

bool CollisionBox::checkColliding(int x2, int y2, int radius) {

	if ((x2+radius > top_left_.x && x2-radius < bottom_right_.x) &&
	(y2+radius > top_left_.y && y2-radius < bottom_right_.x)) {
		return true;
	}
	return false;
}

Point2f CollisionBox::getTopleft() {
	return top_left_;
}
Point2f CollisionBox::getBottomRight() {
	return bottom_right_;
}

