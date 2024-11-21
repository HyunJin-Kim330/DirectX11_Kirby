#include "pch.h"
#include "MakeStarBehavior.h"
#include "NotMoveStar.h"
#include "Kirby.h"

MakeStarBehavior::MakeStarBehavior()
{

}

MakeStarBehavior::~MakeStarBehavior()
{

}

void MakeStarBehavior::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string standardTag = standard->GetTag();
	string otherCollideTag = other->GetTag();

	if (standardTag == "NotMoveStar")
	{
		NotMoveStar* star = dynamic_cast<NotMoveStar*>(standard->GetGameObject());

		if (star->GetSucked() && otherCollideTag == "KirbyBasic")
		{
			Kirby* kirby = dynamic_cast<Kirby*>(other->GetGameObject());
			kirby->AlertCollideWithSuckedStarMonster();
			star->Disappear();
		}
	}
}