#pragma once
#include "CollisionBehavior.h"

class AbilityStarCollisionBehavior : public CollisionBehavior
{
public:
	AbilityStarCollisionBehavior();
	~AbilityStarCollisionBehavior();

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
};

