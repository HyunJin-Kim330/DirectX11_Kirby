#include "pch.h"
#include "02. GameContents/Player/Kirby/Kirby.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "00. Engine/Manager/SceneManager/Scene.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"

void MonsterCollisionBasicBehavior::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	Direction collisionDirection = CollideLeftOrRight(other, standard, colDirection);		// standard cllider를 기준으로 어느 방향에서 부딪혔는지
	Direction contraryDirection = collisionDirection == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
	Monster* monster = dynamic_cast<Monster*>(standard->GetGameObject());

	if (otherColliderTag == "KirbyBasic")
	{
		Kirby* kirby = dynamic_cast<Kirby*>(other->GetGameObject());
		if (monster->GetStatus() == MonsterStatus::Sucked)
		{
			SuckItem item = monster->GetSuckItem();
			
			if (item == SuckItem::Ability)
			{
				kirby->AlertCollideWithSuckedAbilityMonster(monster->GetAbility());
			}
			else if (item == SuckItem::Star)
			{
				kirby->AlertCollideWithSuckedStarMonster();
			}

			monster->Die();
		}
		else
		{
			if (standard->GetTag() != "BossMonster")
			{
				monster->AlertAttacked(collisionDirection, 10.f);
			}
			kirby->AlertCollideWithMonster(contraryDirection, monster->GetDamage(), AttackType::Normal);
		}
	}
	else if (otherColliderTag == "RectGrassGround" || otherColliderTag == "ThinFloor")
	{		
		if (colDirection == CollisionDirection::Bottom)
		{
			monster->SetGrounded(true);

			float distance = other->GetVertices()[0].y - standard->GetVertices()[2].y;
			if (distance > 0.f)
			{
				Vec3 presentPosition = monster->GetTransform()->GetPosition();
				monster->GetTransform()->SetPosition(Vec3{ presentPosition.x, presentPosition.y + distance, 0.f });
			}
		}
		else if (colDirection == CollisionDirection::Left || colDirection == CollisionDirection::Right)
		{
			monster->SetDirection(contraryDirection);
		}
	}
}

void MonsterCollisionBasicBehavior::NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c)
{
	set<pair<string, CollisionDirection>>::iterator iter = collidedTag.find({ "RectGrassGround", CollisionDirection::Bottom });

	if (iter == collidedTag.end())
	{
		Monster* monster = dynamic_cast<Monster*>(c->GetGameObject());
		monster->NotCollideAtAllWithGroundBottom();
	}
}