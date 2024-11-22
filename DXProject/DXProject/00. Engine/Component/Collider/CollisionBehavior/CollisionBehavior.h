#pragma once
#include "00. Engine/Component/Collider/Collider.h"

class CollisionBehavior
{
public:
	CollisionBehavior() {}
	virtual ~CollisionBehavior() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) abstract;
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c) {}

protected:
	Direction CollideLeftOrRight(Collider* other, Collider* standard, CollisionDirection colDirection);		// 부딪힌 방향을 왼쪽과 오른쪽으로만 판단
};
