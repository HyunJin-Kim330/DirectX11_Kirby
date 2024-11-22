#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"

class WandererAI : public MonsterAI
{
public:
	WandererAI(bool _followKirby);
	~WandererAI() {}

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

private:
	bool followKirby = false;
};

