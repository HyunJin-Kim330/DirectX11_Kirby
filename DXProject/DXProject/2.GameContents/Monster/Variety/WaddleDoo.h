#pragma once
#include "Monster.h"

class WaddleDoo : public Monster
{
	using Super = Monster;

public:
	WaddleDoo();
	~WaddleDoo();

public:
	virtual Ability* GetAbility() override;

	virtual void Move() override;
	virtual void Attack() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);

private:
	int moveNum = 1;

private:
	void Walk();
	void WalkAndJump();

};

