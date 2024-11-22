#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class AttackBasic : public CollisionBehavior
{
public:
	AttackBasic();
	~AttackBasic();

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

