#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"
class IdleAndAttackAI : public MonsterAI
{
public:
	IdleAndAttackAI();
	~IdleAndAttackAI();

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

};

