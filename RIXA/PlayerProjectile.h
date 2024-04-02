#pragma once
#include "Projectile.h"
class PlayerProjectile :
	public Projectile
{
public:
	PlayerProjectile(Character* owner, Point2D pos);

	void Update(float deltaTime) override;
	bool CheckCollision() override;
};

