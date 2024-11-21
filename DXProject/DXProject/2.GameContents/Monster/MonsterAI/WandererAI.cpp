#include "pch.h"
#include "Monster.h"
#include "Scene.h"
#include "SceneManager.h"
#include "WandererAI.h"

WandererAI::WandererAI(bool _followKirby)
{
	followKirby = _followKirby;
	presentBehavior = MonsterBehavior::Move;
}

void WandererAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
{
	CheckStatusChange(monster, isAttacked, isSucked);

	if (needStartAttack)
	{
		ChangeBehavior(MonsterBehavior::Attack, monster);
	}

	if (presentBehavior == MonsterBehavior::Attacked)
	{
		monster->Attacked();
	}
	else if(presentBehavior == MonsterBehavior::Sucked)
	{
		monster->Sucked();
	}
	else if (presentBehavior == MonsterBehavior::Move)
	{
		monster->Move();
	}
	else if (presentBehavior == MonsterBehavior::Attack)
	{
		monster->Attack();
	}

	if (followKirby)
	{
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		Vec3 presentPosition = monster->GetTransform()->GetPosition();

		if (kirbyPosition.x < presentPosition.x)
		{
			monster->SetDirection(Direction::LEFT);
		}
		else
		{
			monster->SetDirection(Direction::RIGHT);
		}
	}

	if (monster->endOfBehaivor)
	{
		ChangeBehavior(MonsterBehavior::Move, monster);
	}
}