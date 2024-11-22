#include "pch.h"
#include "02. GameContents/Weapon/CollisionBehavior/AttackElectricShock.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "02. GameContents/Player/Kirby/Kirby.h"
#include "02. GameContents/Weapon/Weapon.h"

void AttackElectricShock::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	string standardTag = standard->GetTag();

	if (standardTag == "BeamWeapon" || standardTag == "SparkWeapon")
	{
		Weapon* waepon = dynamic_cast<Weapon*>(standard->GetGameObject());

		if (otherColliderTag == "MonsterBasic")
		{
			Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());

			if (colDirection == CollisionDirection::Left)
			{
				monster->AlertAttacked(Direction::RIGHT, waepon->GetDamage());
			}
			else
			{
				monster->AlertAttacked(Direction::LEFT, waepon->GetDamage());
			}
		}
		else if (otherColliderTag == "KirbyBasic")
		{
			Kirby* kirby = dynamic_cast<Kirby*>(other->GetGameObject());

			if (colDirection == CollisionDirection::Left)
			{
				kirby->AlertCollideWithMonster(Direction::RIGHT, 30.f, AttackType::Electric);
			}
			else
			{
				kirby->AlertCollideWithMonster(Direction::LEFT, 30.f, AttackType::Electric);
			}
		}
	}

}