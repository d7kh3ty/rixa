#pragma once
#include "Character.h"
class Player :
    public Character
{
public:
    Player(const Point2D pos);

	// Overriden from character base class
	void Update(float deltaTime) override;

	void OnRecieveDamage() override;
	void OnDeath() override;

	void OnLevelCollision() override;

	void UpdateDirection() override; // Remember to run OnDirectionChange

    void HandlePlayerMovement();
	void HandlePlayerShooting(float deltaTime);

	void SetPlayerColour(Play::Colour colour);
	void UpdateDamaged(float deltaTime);

	void Shoot();
	void OnShoot();
private:
	const float AngularSpeedModifier;

	float DamagedCooldown;
	float CurrentDamagedCooldown;
	bool IsDamaged;

};

