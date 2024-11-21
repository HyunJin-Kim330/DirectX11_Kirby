#pragma once
#include "Monster.h"

class IceMonster : public Monster
{
	using Super = Monster;
public:
	IceMonster();
	~IceMonster();

public:
	virtual void Move() override;
	virtual void MoveToKirby();
	virtual void Attack() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);
	virtual Ability* GetAbility() override;

private:
	float goTime = 1.5f;
	float goTimeCalc = 0.f;
	float stopTime = 0.3f;
	float stopTimeCalc = 0.f;
	bool go = true;
};

