#pragma once
#include "Monster.h"

class KingDedede : public Monster
{
	using Super = Monster;
public:
	KingDedede();
	~KingDedede();

public:
	virtual void Move() override;
	virtual void Attack() override;
	virtual void Attacked();
	virtual void MoveToKirby();

	virtual Ability* GetAbility() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);

private:
	int moveNum = 1;
	int attackNum = 1;
	bool makedStar = false;
	bool makedBreath = false;

	int att = 1;
	int mov = 1;
private:
	void Idle();
	void Walk();
	void Fly();
	void DededeJump();

	void HamerTakeDownAttack();
	void HamerJumpTakeDownAttack();
	void CallWaddleDeeAttack();

	void ChooseAttack();
	void ChooseMove();
};

