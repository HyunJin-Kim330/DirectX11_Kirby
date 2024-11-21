#pragma once
#include "CollisionBehavior.h"

class AttackElectricShock : public CollisionBehavior
{
public:
	AttackElectricShock() {}
	~AttackElectricShock() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;

};

