#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class AbilityStarCollisionBehavior : public CollisionBehavior
{
public:
	AbilityStarCollisionBehavior();
	~AbilityStarCollisionBehavior();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;
};

