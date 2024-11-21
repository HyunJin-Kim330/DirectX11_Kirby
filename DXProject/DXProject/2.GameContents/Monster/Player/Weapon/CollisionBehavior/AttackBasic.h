#pragma once
#include "CollisionBehavior.h"

class AttackBasic : public CollisionBehavior
{
public:
	AttackBasic();
	~AttackBasic();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

