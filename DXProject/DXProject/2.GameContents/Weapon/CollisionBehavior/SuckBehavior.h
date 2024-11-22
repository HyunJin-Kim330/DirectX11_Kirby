#pragma once
#include "CollisionBehavior.h"
class SuckBehavior : public CollisionBehavior
{
public:
	SuckBehavior() {}
	~SuckBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
};

