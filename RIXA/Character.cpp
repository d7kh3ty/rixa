#include "Character.h"
#include "Level.h"

Character::Character(int type, Point2D pos, int collisionRadius, const char* spriteName) :
	Actor(type, pos, collisionRadius, spriteName),
	IsDead(false),
	CurrentAttackCooldown(0)
{
	GameObjectRef.owningCharacter = this;
}

void Character::Update(float deltaTime)
{
}

// Apply damage and run OnRecieveDamage()
void Character::ApplyDamage(int damage)
{
	Health -= damage;
	OnRecieveDamage();

	if (Health <= 0)
	{
		SetIsDead(true);
		OnDeath();
	}
}

// Runs when ApplyDamage() run
void Character::OnRecieveDamage()
{
}

// Runs on death
void Character::OnDeath()
{
}

// Checks obstacle collisions, runs OnLevelCollision() if true
bool Character::CheckLevelCollision()
{
	Level* level = Play::GetLevel();
	Point2D position = GameObjectRef.pos;

	for (auto collision : level->getCollisionObjects())
	{
		if (collision.checkColliding(position.x, position.y, GameObjectRef.radius))
		{
			OnLevelCollision();
			return true;
		}
	}

	return false;
}

// Runs when obstacle is collided into
void Character::OnLevelCollision()
{
}

// Update the CurrentDirection enum
// Should run update sprite from direction from here
void Character::UpdateDirection()
{
}

// Characters generally update their sprites based on Current Direction
// This will automatically do that
void Character::OnDirectionChange()
{
	//std::string assetStr = std::string(BaseSprite) + DirectionToString.at(CurrentDirection);
	//const char* assetCStr = assetStr.c_str();

	std::string assetStr;

	switch (CurrentDirection) {
	case Direction::NORTH:
		assetStr = std::string(BaseSprite) + "_north";
		break;
	case Direction::EAST:
		assetStr = std::string(BaseSprite) + "_east";
		break;
	case Direction::NORTHEAST:
		assetStr = std::string(BaseSprite) + "_northeast";
		break;
	case Direction::NORTHWEST:
		assetStr = std::string(BaseSprite) + "_northwest";
		break;
	case Direction::SOUTH:
		assetStr = std::string(BaseSprite) + "_south";
		break;
	case Direction::SOUTHEAST:
		assetStr = std::string(BaseSprite) + "_southeast";
		break;
	case Direction::SOUTHWEST:
		assetStr = std::string(BaseSprite) + "_southwest";
		break;
	case Direction::WEST:
		assetStr = std::string(BaseSprite) + "_west";
		break;
	}

	const char* assetCStr = assetStr.c_str();

	Play::SetSprite(GameObjectRef, assetCStr, AnimSpeed);
}
