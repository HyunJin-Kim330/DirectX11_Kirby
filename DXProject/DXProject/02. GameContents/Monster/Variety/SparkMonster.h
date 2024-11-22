#pragma once
#include "02. GameContents/Monster/Variety/Monster.h"

class SparkMonster : public Monster
{
	using Super = Monster;
public:
	SparkMonster();
	~SparkMonster();

public:
	virtual void Move() override;
	virtual void Attack() override;
	virtual void MoveToKirby();

	virtual Ability* GetAbility() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);

private:
	int moveNum = 1;

private:
	void ShortJump();
	void LongJump();

};

