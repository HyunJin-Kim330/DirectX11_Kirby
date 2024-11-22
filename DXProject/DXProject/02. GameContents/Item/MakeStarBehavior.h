#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class MakeStarBehavior : public CollisionBehavior
{
public:
	MakeStarBehavior();
	~MakeStarBehavior();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

