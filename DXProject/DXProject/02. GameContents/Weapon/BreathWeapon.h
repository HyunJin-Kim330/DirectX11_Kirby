#pragma once
#include "02. GameContents/Weapon/Weapon.h"
class BreathWeapon : public Weapon
{
public:
	BreathWeapon();
	~BreathWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	float colliderUpdateTime = 0.f;

private:
	virtual void UpdateObject() override;
	void BreathAttack();

};

