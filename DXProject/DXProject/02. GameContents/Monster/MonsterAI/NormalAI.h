#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"
#include "02. GameContents/Monster/Variety/Monster.h"

class NormalAI : public MonsterAI
{
public:
	NormalAI();
	~NormalAI() {}

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

private:

};

