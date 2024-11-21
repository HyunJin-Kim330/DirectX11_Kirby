#pragma once
#include "Ability.h"

class Weapon;

class NormalAbility : public Ability
{
public:
	NormalAbility();
	~NormalAbility();

	virtual void Attack() override;

protected:
	virtual void ChangeAbility() override;
	virtual void PushS(Kirby* kirby);
	virtual void PushDownArrow(Kirby* kirby);
	virtual void CollideWithMonster(Direction direction, float damage, AttackType attackType);

private:
	void SuckingAttack(Kirby* kirby);
	void StarSpitOut();

};
