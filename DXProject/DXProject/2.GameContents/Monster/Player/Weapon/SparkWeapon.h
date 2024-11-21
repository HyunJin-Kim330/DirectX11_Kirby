#pragma once
#include "Weapon.h"

class SparkWeapon : public Weapon
{
public:
	SparkWeapon(UserType userType);
	~SparkWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;
};
