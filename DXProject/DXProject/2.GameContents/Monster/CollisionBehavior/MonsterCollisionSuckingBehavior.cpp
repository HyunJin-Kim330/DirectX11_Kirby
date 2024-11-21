#include "pch.h"
#include "Monster.h"
#include "MonsterCollisionSuckingBehavior.h"
#include "Kirby.h"

/*
	���Ͱ� �ٶ󺸴� �������� Ŀ�� �ε����� ����
*/

MonsterCollisionSuckingBehavior::MonsterCollisionSuckingBehavior()
{

}

MonsterCollisionSuckingBehavior::~MonsterCollisionSuckingBehavior()
{

}

void MonsterCollisionSuckingBehavior::Behavior(Collider* c, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = c->GetTag();
	Direction collisionDirection = CollideLeftOrRight(c, standard, colDirection);		// standard cllider�� �������� ��� ���⿡�� �ε�������
	Direction contraryDirection = collisionDirection == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
	Monster* monster = dynamic_cast<Monster*>(standard->GetGameObject());

	if (otherColliderTag == "KirbyBasic")
	{
		Kirby* kirby = dynamic_cast<Kirby*>(c->GetGameObject());
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
			if (kirby->isInvincible()) return;

			if (collisionDirection == monster->GetDirectioin())
			{
				monster->AlertStartAttack();
				kirby->InsideMonsterMouth(monster->GetAttackMaintainTime());
			}
			else
			{
				monster->AlertAttacked(collisionDirection, 10.f);
				kirby->AlertCollideWithMonster(contraryDirection, monster->GetDamage(), AttackType::Normal);
			}
		}
	}
	else if (otherColliderTag == "RectGrassGround")
	{
		if (colDirection == CollisionDirection::Bottom)
		{
			monster->SetGrounded(true);

			float distance = c->GetVertices()[0].y - standard->GetVertices()[2].y;
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

void MonsterCollisionSuckingBehavior::NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c)
{
	set<pair<string, CollisionDirection>>::iterator iter = collidedTag.find({ "RectGrassGround", CollisionDirection::Bottom });

	if (iter == collidedTag.end())
	{
		Monster* monster = dynamic_cast<Monster*>(c->GetGameObject());
		monster->NotCollideAtAllWithGroundBottom();
	}
}