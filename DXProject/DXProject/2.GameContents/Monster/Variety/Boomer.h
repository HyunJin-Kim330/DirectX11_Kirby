#pragma once
#include "Monster.h"

class Boomer : public Monster
{
	using Super = Monster;
public:
	Boomer();
	~Boomer();

public:
	virtual void Move() override;
	virtual void Attack() override;
	virtual Ability* GetAbility() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);

private:
	int attackNum = 1;
	bool weaponTriggerd = false;

private:
	void DicideAttack();
	void ShortAttack();
	void LongAttack();
};

