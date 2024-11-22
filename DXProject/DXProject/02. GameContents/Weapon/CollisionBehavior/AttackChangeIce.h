#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class AttackChangeIce : public CollisionBehavior
{
public:
	AttackChangeIce();
	~AttackChangeIce();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

