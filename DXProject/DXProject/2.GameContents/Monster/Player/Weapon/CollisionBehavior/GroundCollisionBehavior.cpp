#include "pch.h"
#include "GroundCollisionBehavior.h"
#include "Kirby.h"
#include "Ability.h"

void GroundCollisionBehavior::Behavior(Collider* c1, Collider* c2, CollisionDirection colDirection)
{
	if (c2->GetTag() == "KirbyBasic")
	{
		Kirby* kirby = dynamic_cast<Kirby*>(c2->GetGameObject());

		if (colDirection == CollisionDirection::Bottom)
		{
			kirby->SetGrounded(true);
		}
		
	}
}

void GroundCollisionBehavior::NotAtAllCollideBehavior(set<string> collidedTag, Collider* c)
{
	set<string>::iterator iter = collidedTag.find("RectGrassGround");

	if (iter == collidedTag.end())
	{
		if (c->GetTag() == "KirbyBasic")
		{
			Kirby* kirby = dynamic_cast<Kirby*>(c->GetGameObject());

			kirby->SetGrounded(false);
		}
	}
}