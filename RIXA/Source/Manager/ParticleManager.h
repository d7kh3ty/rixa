#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
class ParticleManager
{
public:
	static void AddParticle(const char* asset, Point2D pos, float animSpeed, float scale);

	static void UpdateParticles();

	static void DeleteAllParticles();
};
