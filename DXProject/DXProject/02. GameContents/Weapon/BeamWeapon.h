#pragma once
#include "02. GameContents/Weapon/Weapon.h"

class BeamWeapon : public Weapon
{
public:
	BeamWeapon(UserType userType);
	~BeamWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;
	int angle = 0;
	UserType user = UserType::Kirby;
};

