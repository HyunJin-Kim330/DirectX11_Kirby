#pragma once
#include "02. GameContents/Weapon/Weapon.h"

class StarWeapon : public Weapon
{
public:
	StarWeapon();
	~StarWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;

};

