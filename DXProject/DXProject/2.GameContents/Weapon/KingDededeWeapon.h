#pragma once
#include "Weapon.h"

class KingDededeWeapon : public Weapon
{
public:
	KingDededeWeapon();
	~KingDededeWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;
	int angle = 0;

};
