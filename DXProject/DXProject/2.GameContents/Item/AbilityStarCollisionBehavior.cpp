#include "pch.h"
#include "AbilityStar.h"
#include "AbilityStarCollisionBehavior.h"
#include "Kirby.h"

AbilityStarCollisionBehavior::AbilityStarCollisionBehavior()
{

}

AbilityStarCollisionBehavior::~AbilityStarCollisionBehavior()
{

}

void AbilityStarCollisionBehavior::Behavior(Collider* c, Collider* standard, CollisionDirection colDirection)
{
	string standardTag = standard->GetTag();
	string otherCollideTag = c->GetTag();

	if (standardTag == "AbilityStar")
	{
		AbilityStar* star = dynamic_cast<AbilityStar*>(standard->GetGameObject());

		if (star->GetSucked() && otherCollideTag == "KirbyBasic")
		{
			Kirby* kirby = dynamic_cast<Kirby*>(c->GetGameObject());
			kirby->AlertCollideWithSuckedAbilityMonster(star->GetAbility());
			star->Disappear(false);
		}

		if (otherCollideTag == "RectGrassGround")
		{
			if (colDirection == CollisionDirection::Bottom)
			{
				star->AlertLandGround();
			}
			else if (colDirection == CollisionDirection::Left)
			{
				star->AlertCollideSideGround(Direction::LEFT);
			}
			else if (colDirection == CollisionDirection::Right)
			{
				star->AlertCollideSideGround(Direction::RIGHT);
			}
		}
	}
}