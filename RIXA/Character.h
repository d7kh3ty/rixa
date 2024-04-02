#pragma once
#include "Actor.h"

class Character :
    public Actor
{
public:
    Character(int type, Point2D pos, int collisionRadius, const char* spriteName);

    virtual void Update(float deltaTime) override; // Must override

    // Damage functions
    void ApplyDamage(int damage);
    virtual void OnRecieveDamage(); // Must override

    // Runs on death
    virtual void OnDeath(); // Must override

    // Collision with level
    bool CheckLevelCollision();
    virtual void OnLevelCollision(); // Must override

    // Update sprite based on direction
    virtual void UpdateDirection(); // Must override
    void OnDirectionChange();

    // Getters/Setters
    float GetAttackRange() const { return AttackRange; }
    float GetAttackDamage() const { return AttackDamage; }

    int GetHealth() const { return Health; }

    bool GetIsDead() const { return IsDead; }
    void SetIsDead(bool isDead) { IsDead = isDead; }

protected:
    enum class Direction
    {
        NORTH,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST
    };

    // Define a mapping from enum values to string representations
    //const std::unordered_map<Direction, const char*> DirectionToString{};

    const char* BaseSprite;
    float AnimSpeed;
    Direction CurrentDirection;

    int AttackDamage;
    float AttackRange;
    float AttackInterval;
    float AttackCooldown;
    float CurrentAttackCooldown;
    float ProjectileSpeed;

    bool IsDead;

    int Health;
    float Speed;
};

