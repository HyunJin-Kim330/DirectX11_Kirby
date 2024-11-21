#pragma once
#include "Weapon.h"
class BoomerangWeapon :  public Weapon
{
public:
	BoomerangWeapon(UserType userType);
	~BoomerangWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;
	UserType GetUserType() { return userType; }


private:
	UserType userType;

private:
	virtual void UpdateObject() override;

};

