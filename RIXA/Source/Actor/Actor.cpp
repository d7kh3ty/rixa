#include "Actor.h"

// Uses the same parameters the create the game object
Actor::Actor(int type, const Point2D pos, int collisionRadius, const char* spriteName) : GameObjectID(Play::CreateGameObject(type, pos, collisionRadius, spriteName)), GameObjectRef(Play::GetGameObject(GameObjectID))
{
}