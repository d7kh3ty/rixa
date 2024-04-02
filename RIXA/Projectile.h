#pragma once
#include "Actor.h"

class Character;
class Projectile :
    public Actor
{
    public:
        Projectile(Character* owner, const char* asset, Point2D velocity);
        void Update(float deltaTime) override;
        bool CheckOutOfRange();

        // These should be overriden, as they change the way they are updated
        virtual bool CheckCollision();

        Character* GetOwner() const { return Owner; }
protected:
    Character* Owner;
    bool CanDamage;
};

