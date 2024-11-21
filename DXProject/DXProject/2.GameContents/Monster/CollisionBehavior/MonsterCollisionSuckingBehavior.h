#pragma once
#include "CollisionBehavior.h"
class MonsterCollisionSuckingBehavior : public CollisionBehavior
{
public:
	MonsterCollisionSuckingBehavior();
	~MonsterCollisionSuckingBehavior();

	virtual void Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection) override;
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c);

};

