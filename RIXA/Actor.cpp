#include "Actor.h"

//Actor::Actor(int type, Point2D pos, int collisionRadius, const char* spriteName) :
//	GameObjectID(Play::CreateGameObject(type, pos, collisionRadius, spriteName)//,
//	//GameObjectRef(Play::GetGameObject(GameObjectID))
//{
//	GameObjectRef = Play::GetGameObject(GameObjectID);
//}

Actor::Actor(int type, Point2D pos, int collisionRadius, const char* spriteName) : GameObjectID(Play::CreateGameObject(type, pos, collisionRadius, spriteName)), GameObjectRef(Play::GetGameObject(GameObjectID))
{
	//GameObjectID = Play::CreateGameObject(type, pos, collisionRadius, spriteName);

}