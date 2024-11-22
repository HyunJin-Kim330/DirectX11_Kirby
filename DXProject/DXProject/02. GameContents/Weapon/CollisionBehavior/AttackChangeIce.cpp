#include "pch.h"
#include "02. GameContents/Weapon/CollisionBehavior/AttackChangeIce.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "02. GameContents/Item/IceBlock.h"
#include "02. GameContents/Weapon/Weapon.h"

AttackChangeIce::AttackChangeIce()
{

}

AttackChangeIce::~AttackChangeIce()
{

}

void AttackChangeIce::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	string standardTag = standard->GetTag();

	if (otherColliderTag == "MonsterBasic")
	{
		Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());

		Vec3 monsterPosition = monster->GetMidstPosition();
		monster->AddGameObjectToScene(new IceBlock(monsterPosition));
		monster->Die();
	}
	else if (otherColliderTag == "KirbyBasic")
	{

	}
	else if (otherColliderTag == "BossMonster")
	{
		Monster* monster = dynamic_cast<Monster*>(other->GetGameObject());
		Weapon* waepon = dynamic_cast<Weapon*>(standard->GetGameObject());

		Vec3 monsterPosition = monster->GetMidstPosition();
		if (colDirection == CollisionDirection::Left)
		{
			monster->AlertAttacked(Direction::RIGHT, waepon->GetDamage());
		}
		else
		{
			monster->AlertAttacked(Direction::LEFT, waepon->GetDamage());
		}
	}
}