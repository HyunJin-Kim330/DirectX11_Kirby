#pragma once
#include "CollisionBehavior.h"

class AttackDisappearBehavior : public CollisionBehavior
{
public:
	AttackDisappearBehavior() {}
	AttackDisappearBehavior(wstring _disappearEffect) { disappearEffect = _disappearEffect; };
	~AttackDisappearBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;

private:
	wstring disappearEffect = L"-";
};

