#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class AttackElectricShock : public CollisionBehavior
{
public:
	AttackElectricShock() {}
	~AttackElectricShock() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

};

