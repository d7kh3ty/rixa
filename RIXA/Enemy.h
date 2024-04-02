#pragma once
#include "Character.h"
//#include "EnemyBullet.h"

//class EnemyBullet;
class Enemy :
    public Character
{
public:
    Enemy(int type, Point2D pos, int collisionRadius, const char* spriteName);

    void Update(float deltaTime) override;

    void OnRecieveDamage() override;
    void OnDeath() override;

    void OnLevelCollision() override;

    void UpdateDirection() override;

    // Shooting
    void TryShoot(Point2D velocity);
    bool CanShoot(float deltaTime);

    // Movement
    void UpdateMovement();

    // Out of bounds
    void CheckOutOfBounds();


protected:
    int DetectionRange;// = 250;
    bool PlayerDetected;// = false;

private:
    float DirectionTolerance;
};