#pragma once // Headers are included once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

// Actors are just a nifty wrapper around the GameObject and its corresponding ID
class Actor
{
public:
	Actor(int type, const Point2D pos, int collisionRadius, const char* spriteName);

	int GetID() const { return GameObjectID; }
	GameObject& GetGameObject() const { return GameObjectRef; }

	virtual void Update(float deltaTime) { return; }

protected:
	const int GameObjectID;
	GameObject& GameObjectRef;
};

