#include "pch.h"
#include "MonsterAI.h"
#include "Monster.h"

MonsterAI::MonsterAI()
{

}

MonsterAI::~MonsterAI()
{

}

void MonsterAI::ChangeBehavior(MonsterBehavior newBehaivor, Monster* monster)
{
	presentBehavior = newBehaivor;
	monster->ChangeBehavior(presentBehavior);
}

void MonsterAI::ChangeNextBehavior(Monster* monster)
{
	behaviorOrderIter++;
	if (behaviorOrderIter == behaviorOrder.end())
	{
		behaviorOrderIter = behaviorOrder.begin();
	}

	ChangeBehavior(*behaviorOrderIter, monster);
}

void MonsterAI::CheckStatusChange(Monster* monster, bool isAttacked, bool isSucked)
{
	if (isAttacked)
	{
		ChangeBehavior(MonsterBehavior::Attacked, monster);
	}

	if (isSucked)
	{
		ChangeBehavior(MonsterBehavior::Sucked, monster);
	}
}