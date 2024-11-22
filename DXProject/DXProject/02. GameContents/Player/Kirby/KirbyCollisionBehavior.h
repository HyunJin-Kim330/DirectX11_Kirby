#pragma once
#include "00. Engine/Component/Collider/CollisionBehavior/CollisionBehavior.h"

class Collision;

class KirbyCollisionBehavior : public CollisionBehavior
{
public:
	KirbyCollisionBehavior() {}
	~KirbyCollisionBehavior() {}

	virtual void Behavior(Collider* other, Collider* standard, CollisionDirection colDirection) override;
	virtual void NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c);
};

