#pragma once
#include "Projectile.h"
class EnemyProjectile :
    public Projectile
{
public:
    EnemyProjectile(Character* owner, Point2D velocity);
    bool CheckCollision() override;
};

