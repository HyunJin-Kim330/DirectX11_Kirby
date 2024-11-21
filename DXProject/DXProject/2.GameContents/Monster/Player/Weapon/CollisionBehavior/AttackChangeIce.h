#pragma once
#include "CollisionBehavior.h"
class AttackChangeIce : public CollisionBehavior
{
public:
	AttackChangeIce();
	~AttackChangeIce();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

