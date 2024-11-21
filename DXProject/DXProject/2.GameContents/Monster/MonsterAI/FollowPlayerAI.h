#pragma once
#include "MonsterAI.h"
#include "Collider.h"
#include "EmptyObject.h"
#include "RectCollider.h"

class FollowPlayerAI : public MonsterAI
{
public:
	FollowPlayerAI();
	~FollowPlayerAI() {}

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

};

