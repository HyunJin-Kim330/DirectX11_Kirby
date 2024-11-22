#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class SuckBehavior : public CollisionBehavior
{
public:
	SuckBehavior() {}
	~SuckBehavior() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;
};

