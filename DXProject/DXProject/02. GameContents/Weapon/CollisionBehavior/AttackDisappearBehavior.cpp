#include "pch.h"
#include "02. GameContents/Weapon/CollisionBehavior/AttackDisappearBehavior.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "02. GameContents/Weapon/BoomerangWeapon.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Player/Kirby/Kirby.h"
#include "search.h"

void AttackDisappearBehavior::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	string standardTag = standard->GetTag();

	if (standardTag == "BoomerangWeapon")
	{
		BoomerangWeapon* boomerang = dynamic_cast<BoomerangWeapon*>(standard->GetGameObject());
		UserType user = boomerang->GetUserType();

		if ((user == UserType::Monster && otherColliderTag == "MonsterBasic") ||
			(user == UserType::Kirby && otherColliderTag == "KirbyBasic"))
		{
			if (boomerang->GetAttackTime() < 2.f) return;
			GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(standard->GetGameObject());
			boomerang->StopAttack(true);
		}
		else
		{
			if (otherColliderTag == "MonsterBasic" || otherColliderTag == "BossMonster")
			{
				Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());

				if (colDirection == CollisionDirection::Left)
				{
					monster->AlertAttacked(Direction::RIGHT, boomerang->GetDamage());
				}
				else
				{
					monster->AlertAttacked(Direction::LEFT, boomerang->GetDamage());
				}
			}
			else if (otherColliderTag == "KirbyBasic")
			{
				Kirby* kirby = dynamic_cast<Kirby*>(other->GetGameObject());

				if (colDirection == CollisionDirection::Left)
				{
					kirby->AlertCollideWithMonster(Direction::RIGHT, 10.f, AttackType::Normal);
				}
				else
				{
					kirby->AlertCollideWithMonster(Direction::LEFT, 10.f, AttackType::Normal);
				}
			}

			if (disappearEffect != L"-")
			{
				GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(disappearEffect, standard->GetGameObject()->GetMidstPosition()));
			}
			GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(standard->GetGameObject());
			boomerang->StopAttack(true);
		}

	}
	else
	{
		if (otherColliderTag == "MonsterBasic" || otherColliderTag == "BossMonster")
		{
			Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());
			Weapon* waepon = dynamic_cast<Weapon*>(standard->GetGameObject());

			if (colDirection == CollisionDirection::Left)
			{
				monster->AlertAttacked(Direction::RIGHT, waepon->GetDamage());
			}
			else
			{
				monster->AlertAttacked(Direction::LEFT, waepon->GetDamage());
			}
		}

		if (disappearEffect != L"-")
		{
			GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(disappearEffect, standard->GetGameObject()->GetMidstPosition()));
		}
		GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(standard->GetGameObject());
	}
}