#include "pch.h"
#include "02. GameContents/Weapon/CollisionBehavior/SuckBehavior.h"
#include "02. GameContents/Weapon/NormalWeapon.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "02. GameContents/Item/AbilityStar.h"
#include "02. GameContents/Item/NotMoveStar.h"

void SuckBehavior::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();

	if (standard->GetTag() == "NormalWeapon")
	{
		NormalWeapon* normalWeapon = dynamic_cast<NormalWeapon*>(standard->GetGameObject());
		Vec3 summonerPosition = normalWeapon->GetSummonerPosition();

		if (otherColliderTag == "MonsterBasic")
		{
			Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());
			if (monster->GetSuckable())
			{
				monster->AlertSucked(Direction::LEFT, summonerPosition);
			}
		}
		else if (otherColliderTag == "AbilityStar")
		{
			AbilityStar* star = dynamic_cast<AbilityStar*>(other->GetGameObject());
			star->AlertSucked(summonerPosition);
		}
		else if (otherColliderTag == "NotMoveStar")
		{
			NotMoveStar* star = dynamic_cast<NotMoveStar*>(other->GetGameObject());
			star->AlertSucked(summonerPosition);
		}
	}
}