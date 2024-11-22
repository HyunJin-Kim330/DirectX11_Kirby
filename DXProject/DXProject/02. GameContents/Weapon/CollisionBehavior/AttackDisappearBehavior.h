#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class AttackDisappearBehavior : public CollisionBehavior
{
public:
	AttackDisappearBehavior() {}
	AttackDisappearBehavior(wstring _disappearEffect) { disappearEffect = _disappearEffect; };
	~AttackDisappearBehavior() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;

private:
	wstring disappearEffect = L"-";
};

