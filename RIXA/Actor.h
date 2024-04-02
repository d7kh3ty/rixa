#pragma once // Headers are included once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

class Actor
{
public:
	Actor(int type, Point2D pos, int collisionRadius, const char* spriteName);

	int GetID() const { return GameObjectID; }
	GameObject& GetGameObject() const { return GameObjectRef; }

	virtual void Update(float deltaTime) { return; }

protected:
	int GameObjectID;
	GameObject& GameObjectRef;
};

