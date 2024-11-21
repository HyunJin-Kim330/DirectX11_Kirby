#pragma once
#include "CollisionBehavior.h"

class MonsterCollisionBasicBehavior : public CollisionBehavior
{
public:
	MonsterCollisionBasicBehavior() {}
	~MonsterCollisionBasicBehavior() {}

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
	//virtual void NotAtAllCollideBehavior(set<string> collidedTag, Collider* c);
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c);
};

