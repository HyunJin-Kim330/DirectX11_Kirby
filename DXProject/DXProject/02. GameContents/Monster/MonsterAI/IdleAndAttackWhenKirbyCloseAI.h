#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"
class IdleAndAttackWhenKirbyCloseAI : public MonsterAI
{
public:
	IdleAndAttackWhenKirbyCloseAI(float attackDistance = 3.f);
	~IdleAndAttackWhenKirbyCloseAI();

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

private:
	float attackDistance = 3.f;


};

