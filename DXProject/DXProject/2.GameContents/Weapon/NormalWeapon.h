#pragma once
#include "Weapon.h"

class NormalWeapon : public Weapon
{
public:
	NormalWeapon();
	~NormalWeapon();

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) override;

private:
	virtual void UpdateObject() override;
	void BreathAttack();
	void SuckAttack();
	void SetInitialCollider();
	void SetInitialPosition();
};

