#pragma once
#include "02. GameContents/Monster/MonsterAI/MonsterAI.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/GameObject/EmptyObject.h"
#include "00. Engine/Component/Collider/RectCollider.h"

class FollowPlayerAI : public MonsterAI
{
public:
	FollowPlayerAI();
	~FollowPlayerAI() {}

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) override;

};

