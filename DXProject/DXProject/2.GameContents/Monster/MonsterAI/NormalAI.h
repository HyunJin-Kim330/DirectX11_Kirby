#pragma once
#include "MonsterAI.h"
#include "Monster.h"

class NormalAI : public MonsterAI
{
public:
	NormalAI();
	~NormalAI() {}

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

private:

};

