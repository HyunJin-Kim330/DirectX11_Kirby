#include "pch.h"
#include "FollowPlayerRandomBehaviorAI.h"
#include <random>
#include "Monster.h"
#include "Scene.h"
#include "SceneManager.h"

FollowPlayerRandomBehaviorAI::FollowPlayerRandomBehaviorAI()
{
	presentBehavior = MonsterBehavior::Move;
}

FollowPlayerRandomBehaviorAI::~FollowPlayerRandomBehaviorAI()
{
	
}

void FollowPlayerRandomBehaviorAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
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

		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(1, 2);
		int ret = dist(mt);
		
		if(ret == 1) ChangeBehavior(MonsterBehavior::Attack, monster);
		else if(ret == 2) ChangeBehavior(MonsterBehavior::Move, monster);

	}
}