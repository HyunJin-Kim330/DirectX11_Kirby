#include "pch.h"
#include "AttackBasic.h"
#include "Kirby.h"
#include "Weapon.h"

AttackBasic::AttackBasic()
{

}

AttackBasic::~AttackBasic()
{

}

void AttackBasic::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	string standardTag = standard->GetTag();

	if (otherColliderTag == "KirbyBasic")
	{
		Kirby* kirby = dynamic_cast<Kirby*>(other->GetGameObject());
		Weapon* waepon = dynamic_cast<Weapon*>(standard->GetGameObject());

		if (colDirection == CollisionDirection::Left)
		{
			kirby->AlertCollideWithMonster(Direction::RIGHT, waepon->GetDamage(), AttackType::Normal);
		}
		else
		{
			kirby->AlertCollideWithMonster(Direction::LEFT, waepon->GetDamage(), AttackType::Normal);
		}
	}
}