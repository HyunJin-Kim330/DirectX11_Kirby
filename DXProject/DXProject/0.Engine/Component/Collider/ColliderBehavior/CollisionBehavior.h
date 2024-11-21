#pragma once
#include "Collider.h"

class CollisionBehavior
{
public:
	CollisionBehavior() {}
	virtual ~CollisionBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) abstract;
	//virtual void NotAtAllCollideBehavior(set<string> collidedTag, Collider* c) {}
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c) {}


protected:
	Direction CollideLeftOrRight(Collider* citizen, Collider* standard, CollisionDirection colDirection);
};
