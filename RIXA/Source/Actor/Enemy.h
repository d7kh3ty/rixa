#pragma once
#include "Character.h"
//#include "EnemyBullet.h"

//class EnemyBullet;
class Enemy :
    public Character
{
public:
    Enemy(int type, const Point2D pos, int collisionRadius, const char* spriteName);

    void Update(float deltaTime) override;

    void OnRecieveDamage() override;
    void OnDeath() override;

    void OnLevelCollision() override;

    void UpdateDirection() override;

    // Shooting
    void TryShoot(const Point2D velocity);
    bool CanShoot(float deltaTime);

    // Movement
    void UpdateMovement();


protected:
    int DetectionRange;
    bool PlayerDetected;

private:
    float DirectionTolerance;
};