#pragma once
#include "02. GameContents/Monster/Variety/Monster.h"

class WaddleDee : public Monster
{
	using Super = Monster;
public:
	WaddleDee(bool umbrella = false);
	~WaddleDee();

public:
	virtual void Move() override;
	virtual void Attack() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);
	virtual Ability* GetAbility() override;

private:
};

