#pragma once
#include "CollisionBehavior.h"

class MakeStarBehavior : public CollisionBehavior
{
public:
	MakeStarBehavior();
	~MakeStarBehavior();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

