#pragma once
#include "CollisionBehavior.h"
class Collision;

class GroundCollisionBehavior : public CollisionBehavior
{
public:
	GroundCollisionBehavior() {}
	~GroundCollisionBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
	virtual void NotAtAllCollideBehavior(set<string> collidedTag, Collider* c);
};

