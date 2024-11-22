#include "pch.h"
#include "02. GameContents/Monster/MonsterAI/FollowPlayerAI.h"
#include "00. Engine/Manager/SceneManager/Scene.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Monster/Variety/Monster.h"

// 커비 방향으로 움직이며, 일정거리로 가까워질 경우 공격

FollowPlayerAI::FollowPlayerAI()
{
	presentBehavior = MonsterBehavior::Move;
}

void FollowPlayerAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
{
	CheckStatusChange(monster, isAttacked, isSucked);

	if (presentBehavior == MonsterBehavior::Move)
	{
		monster->MoveToKirby();
	}
	else if (presentBehavior == MonsterBehavior::Attack)
	{
		monster->Attack();
	}
	else if (presentBehavior == MonsterBehavior::Attacked)
	{
		monster->Attacked();
	}
	else if (presentBehavior == MonsterBehavior::Sucked)
	{
		monster->Sucked();
	}

	if (monster->endOfBehaivor)
	{
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		Vec3 monsterPosition = monster->GetTransform()->GetPosition();

		if (kirbyPosition.x < monsterPosition.x)
		{
			monster->SetDirection(Direction::LEFT);
		}
		else
		{
			monster->SetDirection(Direction::RIGHT);
		}

		float distanceMonsterWithKirby = 0.f;
		float a = kirbyPosition.x - monsterPosition.x;
		float b = kirbyPosition.y - monsterPosition.y;
		distanceMonsterWithKirby = sqrtf(a * a + b * b);

		if (distanceMonsterWithKirby <= 2.5f)
		{
			ChangeBehavior(MonsterBehavior::Attack, monster);
		}
		else
		{
			ChangeBehavior(MonsterBehavior::Move, monster);
		}
	}
}