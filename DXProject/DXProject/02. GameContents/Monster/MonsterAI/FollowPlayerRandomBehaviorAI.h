#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"
class FollowPlayerRandomBehaviorAI : public MonsterAI
{
public:
	FollowPlayerRandomBehaviorAI();
	~FollowPlayerRandomBehaviorAI();

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

};

