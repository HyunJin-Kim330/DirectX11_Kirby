#pragma once
#include "CollisionBehavior.h"
class Collision;

class KirbyCollisionBehavior : public CollisionBehavior
{
public:
	KirbyCollisionBehavior() {}
	~KirbyCollisionBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c);
};

