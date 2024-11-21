#pragma once
#include "Monster.h"
class GreenMonster : public Monster
{
	using Super = Monster;
public:
	GreenMonster();
	~GreenMonster();

public:
	virtual void Move() override;
	virtual void Attack() override;

	virtual Ability* GetAbility() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);

private:
	int moveNum = 1;
	bool angry = false;

private:
	void Idle();
	void Walk();
}; 

