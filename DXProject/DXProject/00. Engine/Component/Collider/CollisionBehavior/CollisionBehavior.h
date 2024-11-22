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
	Direction CollideLeftOrRight(Collider* other, Collider* standard, CollisionDirection colDirection);		// �ε��� ������ ���ʰ� ���������θ� �Ǵ�
};
