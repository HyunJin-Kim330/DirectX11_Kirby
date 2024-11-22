#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class MonsterCollisionBasicBehavior : public CollisionBehavior
{
public:
	MonsterCollisionBasicBehavior() {}
	~MonsterCollisionBasicBehavior() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c);
};

