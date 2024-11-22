#pragma once
#include "02. GameContents/Weapon/Weapon.h"

class IceWeapon : public Weapon
{
public:
	IceWeapon(UserType userType);
	~IceWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;
	void SetInitialPosition();
	void SetInitialCollider();

};

