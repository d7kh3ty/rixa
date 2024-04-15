#pragma once
#include "Projectile.h"
class EnemyProjectile :
    public Projectile
{
public:
    EnemyProjectile(Character& owner, const Point2D velocity);
    bool CheckCollision() override;
};

