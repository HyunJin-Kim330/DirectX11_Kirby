#include "pch.h"
#include "02. GameContents/Monster/MonsterAI/IdleAndAttackAI.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "00. Engine/Manager/SceneManager/Scene.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include <random>

// 이동하지 않으며, '서있기->공격'을 반복, 커비 인식x

IdleAndAttackAI::IdleAndAttackAI()
{
	behaviorOrder.push_back(MonsterBehavior::Idle);
	behaviorOrder.push_back(MonsterBehavior::Attack);
	behaviorOrderIter = behaviorOrder.begin();

	presentBehavior = *behaviorOrderIter;
}

IdleAndAttackAI::~IdleAndAttackAI()
{

}

void IdleAndAttackAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
{
	CheckStatusChange(monster, isAttacked, isSucked);

	if (presentBehavior == MonsterBehavior::Idle)
	{
		monster->Idle();
	}
	else if (presentBehavior == MonsterBehavior::Move)
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
		if (presentBehavior == MonsterBehavior::Attack)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_int_distribution<int> dist(0, 1);
			if (dist(mt) == 0) monster->SetDirection(Direction::LEFT);
			else monster->SetDirection(Direction::RIGHT);
		}

		if (presentBehavior == MonsterBehavior::Attacked)
		{
			behaviorOrderIter = behaviorOrder.begin();
		}
		else
		{
			ChangeNextBehavior(monster);
		}
	}
}