#include "pch.h"
#include "AttackElectricShock.h"
#include "Monster.h"
#include "Kirby.h"
#include "Weapon.h"

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