#pragma once
#include "MonsterAI.h"
class FollowPlayerRandomBehaviorAI : public MonsterAI
{
public:
	FollowPlayerRandomBehaviorAI();
	~FollowPlayerRandomBehaviorAI();

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

};

